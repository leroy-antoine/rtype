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
            if (size >= rtype::config::BOSS_WIDTH * 0.8f) {
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

        if (found == _entities.end()) {
            return;
        }
        _reg.destroy(found->second);
        _entities.erase(found);
    }

    void Replica::apply(const std::vector<rtype::EntityState> &states)
    {
        for (const rtype::EntityState &state : states) {
            apply(state);
        }
    }

    void Replica::apply(const rtype::EntityState &state)
    {
        const auto found = _entities.find(state.id);

        if (found == _entities.end()) {
            return;
        }
        gameLogic::Vec2Pos &pos = _reg.get<gameLogic::Vec2Pos>(found->second);

        _reg.get<gameLogic::Vec2Velocity>(found->second) =
            gameLogic::Vec2Velocity{{state.pos.x - pos.x, state.pos.y - pos.y}};
        pos = state.pos;
    }

    void Replica::update(const float dt)
    {
        _reg.each<rtype::Lifetime>([dt](Engine::Entity, rtype::Lifetime &lifetime) {
            lifetime.remaining -= dt;
            if (lifetime.remaining < 0.f) {
                lifetime.remaining = 0.f;
            }
        });
    }

    void Replica::apply(const rtype::GameStatus &status)
    {
        _status = status;
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

    std::vector<Cue> Replica::takeCues()
    {
        return std::exchange(_cues, {});
    }

    void Replica::recordSpawnCue(const rtype::EntitySpawned &spawn)
    {
        switch (spawn.sprite) {
            case rtype::SpriteId::PlayerBullet:
                _cues.push_back(Cue::PlayerShot);
                break;
            case rtype::SpriteId::Boss:
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
