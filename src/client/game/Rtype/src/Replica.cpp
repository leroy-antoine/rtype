#include <utility>
#include "logic/Config.hpp"
#include "client/Replica.hpp"

namespace client {
    namespace {
        // Les explosions de coup sont petites, celle du boss est enorme.
        Cue explosionCue(const float size)
        {
            if (size <= rtype::config::HIT_EXPLOSION_SIZE) {
                return Cue::Hit;
            }
            if (size >= rtype::config::BOSS_EXPLOSION_MIN_SIZE) {
                return Cue::BossExplosion;
            }
            return Cue::Explosion;
        }
    }

    void Replica::apply(const rtype::EntitySpawned &spawn)
    {
        apply(rtype::EntityDestroyed{spawn.id});
        const Engine::Entity e = _reg.create();

        _reg.add(e, spawn.pos);
        _reg.add(e, spawn.box);
        _reg.add(e, gameLogic::Vec2Velocity{});
        _reg.add(e, rtype::Renderable{spawn.sprite});
        if (spawn.sprite == rtype::SpriteId::Player) {
            _reg.add(e, rtype::PlayerTag{spawn.slot});
            if (_slot && spawn.slot == *_slot) {
                _prediction.reset();
            }
        }
        if (spawn.sprite == rtype::SpriteId::Explosion) {
            _reg.add(e, rtype::Lifetime{});
        }
        _entities[spawn.id] = e;
        recordSpawnCue(spawn);
    }

    void Replica::apply(const rtype::EntityDestroyed &destroy)
    {
        const auto found = _entities.find(destroy.id);

        _samples.erase(destroy.id);
        if (found == _entities.end()) {
            return;
        }
        _reg.destroy(found->second);
        _entities.erase(found);
    }

    void Replica::apply(const rtype::protocol::StatesSnapshot &snapshot)
    {
        const auto tick = static_cast<double>(snapshot.tick);

        if (!_latestTick || tick > *_latestTick) {
            _latestTick = tick;
        }
        for (const rtype::EntityState &state : snapshot.states) {
            record(tick, state);
        }
    }

    void Replica::record(const double tick, const rtype::EntityState &state)
    {
        if (!_entities.contains(state.id)) {
            return;
        }
        std::deque<Sample> &samples = _samples[state.id];

        if (samples.empty() || tick > samples.back().tick) {
            samples.push_back({tick, state.pos});
        }
    }

    void Replica::update(const float dt)
    {
        advanceClock(dt);
        interpolate();
        _prediction.update(dt);
        applyPrediction();
        _reg.each<rtype::Lifetime>([dt](Engine::Entity, rtype::Lifetime &lifetime) {
            lifetime.remaining -= dt;
            if (lifetime.remaining < 0.f) {
                lifetime.remaining = 0.f;
            }
        });
    }

    void Replica::advanceClock(const float dt)
    {
        if (!_latestTick) {
            return;
        }
        const double target = *_latestTick - INTERPOLATION_DELAY;

        if (!_renderTick || std::abs(target - *_renderTick) > MAX_CLOCK_ERROR) {
            _renderTick = target;
            return;
        }
        *_renderTick += static_cast<double>(dt) * rtype::config::TICK_RATE;
        *_renderTick += (target - *_renderTick) * std::min(1.0, CLOCK_CORRECTION_RATE * dt);
    }

    void Replica::interpolate()
    {
        if (!_renderTick) {
            return;
        }
        for (auto &[id, samples] : _samples) {
            const auto found = _entities.find(id);

            if (found != _entities.end() && !samples.empty()) {
                interpolate(found->second, samples);
            }
        }
    }

    void Replica::interpolate(const Engine::Entity e, std::deque<Sample> &samples)
    {
        const double now = *_renderTick;

        while (samples.size() > 2 && samples[1].tick <= now) {
            samples.pop_front();
        }
        const Sample &from = samples.front();
        const Sample &to = samples.size() > 1 ? samples[1] : samples.front();
        const double span = to.tick - from.tick;
        const auto t = static_cast<float>(span > 0.0 ? std::clamp((now - from.tick) / span, 0.0, 1.0) : 1.0);

        _reg.get<gameLogic::Vec2Pos>(e) = gameLogic::Vec2Pos{{
            from.pos.x + (to.pos.x - from.pos.x) * t,
            from.pos.y + (to.pos.y - from.pos.y) * t}};
        _reg.get<gameLogic::Vec2Velocity>(e) =
            gameLogic::Vec2Velocity{{to.pos.x - from.pos.x, to.pos.y - from.pos.y}};
    }

    void Replica::apply(const rtype::protocol::OwnState &state)
    {
        _prediction.reconcile(state);
    }

    std::uint32_t Replica::predict(const rtype::PlayerInput &input, const float dt)
    {
        refreshSolids();
        return _prediction.record(input, dt);
    }

    void Replica::applyPrediction()
    {
        const std::optional<gameLogic::Vec2Pos> predicted = _prediction.position();
        const std::optional<Engine::Entity> player = localPlayer();

        if (!predicted || !player) {
            return;
        }
        _reg.get<gameLogic::Vec2Pos>(*player) = *predicted;
        _reg.get<gameLogic::Vec2Velocity>(*player) = _prediction.velocity();
    }

    std::optional<Engine::Entity> Replica::localPlayer()
    {
        std::optional<Engine::Entity> found;

        if (!_slot) {
            return std::nullopt;
        }
        _reg.each<rtype::PlayerTag>([this, &found](const Engine::Entity e, const rtype::PlayerTag &tag) {
            if (tag.slot == *_slot) {
                found = e;
            }
        });
        return found;
    }

    void Replica::apply(const rtype::GameStatus &status)
    {
        if (_status && status.coins > _status->coins) {
            _cues.push_back(Cue::CoinPickup);
        }
        _status = status;
    }

    void Replica::apply(const rtype::AllRecords &records)
    {
        _records = records;
    }

    void Replica::apply(const rtype::protocol::HistoryPage &page)
    {
        if (page.start == 0) {
            _history.clear();
        }
        if (page.start != _history.size()) {
            return;
        }
        _history.insert(_history.end(), page.entries.begin(),
            page.entries.end());
        for (std::size_t map = 0; map < rtype::MAP_COUNT; ++map) {
            _runs[map] = rtype::runsOf(_history,
                static_cast<rtype::MapId>(map));
        }
    }

    const rtype::MapRecords &Replica::runs(const rtype::MapId map) const
    {
        return _runs[map];
    }

    void Replica::moveMapCursor(const int delta)
    {
        const int count = static_cast<int>(ENTRY_COUNT);
        const int next = (static_cast<int>(_mapCursor) + delta + count) % count;

        _mapCursor = static_cast<rtype::MapId>(next);
    }

    bool Replica::onLeaderboard() const
    {
        return _mapCursor == LEADERBOARD_ENTRY;
    }

    LobbyMenu &Replica::menu()
    {
        return _menu;
    }

    const LobbyMenu &Replica::menu() const
    {
        return _menu;
    }

    const rtype::AllRecords &Replica::records() const
    {
        return _records;
    }

    rtype::MapId Replica::mapCursor() const
    {
        return _mapCursor;
    }

    const Prediction &Replica::prediction() const
    {
        return _prediction;
    }

    void Replica::refreshSolids()
    {
        std::vector<rtype::Collider> solids;
        const std::optional<Engine::Entity> player = localPlayer();

        _reg.each<rtype::Renderable, gameLogic::Vec2Pos, gameLogic::Vec2Hitbox>(
            [&solids](const Engine::Entity e, const rtype::Renderable &sprite,
                const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box) {
                if (sprite.id == rtype::SpriteId::RockWall) {
                    solids.push_back({e, pos, box, {}});
                }
            });
        if (player) {
            _prediction.setWorld(_reg.get<gameLogic::Vec2Hitbox>(*player),
                std::move(solids));
        }
    }

    std::optional<rtype::SpriteId> Replica::bossSprite()
    {
        std::optional<rtype::SpriteId> found;

        _reg.each<rtype::Renderable>(
            [&found](const Engine::Entity, const rtype::Renderable &sprite) {
                if (rtype::isBossSprite(sprite.id)) {
                    found = sprite.id;
                }
            });
        return found;
    }

    bool Replica::isHost() const
    {
        if (!_status || !_slot || _status->players.empty()) {
            return false;
        }
        return _status->players.front().slot == *_slot;
    }

    void Replica::apply(const rtype::protocol::OwnedAugment &augments)
    {
        _augments = augments;
    }

    void Replica::apply(const std::vector<rtype::AugmentId> &shopOffers)
    {
        _shopOffers = shopOffers;
    }

    void Replica::apply(const rtype::protocol::BuyResult &result)
    {
        _buyResult = result;
        _cues.push_back(result.ok ? Cue::Purchase : Cue::PurchaseRefused);
    }

    void Replica::assignSlot(const int slot)
    {
        _slot = slot;
    }

    Engine::Registry &Replica::registry()
    {
        return _reg;
    }

    const std::optional<rtype::GameStatus> &Replica::status() const
    {
        return _status;
    }

    std::optional<int> Replica::slot() const
    {
        return _slot;
    }

    const rtype::protocol::OwnedAugment &Replica::augments() const
    {
        return _augments;
    }

    const std::vector<rtype::AugmentId> &Replica::shopOffers() const
    {
        return _shopOffers;
    }

    std::vector<Cue> Replica::takeCues()
    {
        return std::exchange(_cues, {});
    }

    std::optional<rtype::protocol::BuyResult> Replica::takeBuyResult()
    {
        return std::exchange(_buyResult, std::nullopt);
    }

    void Replica::recordSpawnCue(const rtype::EntitySpawned &spawn)
    {
        switch (spawn.sprite) {
            case rtype::SpriteId::PlayerBullet:
                _cues.push_back(Cue::PlayerShot);
                break;
            case rtype::SpriteId::Boss:
            case rtype::SpriteId::DasherBoss:
            case rtype::SpriteId::SummonerBoss:
            case rtype::SpriteId::FortressBoss:
                _cues.push_back(Cue::BossAppears);
                break;
            case rtype::SpriteId::Explosion:
                _cues.push_back(explosionCue(spawn.box.x));
                break;
            default:
                break;
        }
    }
}
