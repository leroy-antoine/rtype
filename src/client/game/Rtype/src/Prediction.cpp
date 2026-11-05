#include <cmath>
#include "logic/Config.hpp"
#include <utility>
#include "logic/Movements.hpp"
#include "logic/Solids.hpp"
#include "client/Prediction.hpp"

namespace client {
    std::uint32_t Prediction::record(const rtype::PlayerInput &input, const float dt)
    {
        const std::uint32_t sequence = _nextSequence++;

        _pending.push_back({sequence, input, dt});
        if (_pending.size() > MAX_PENDING) {
            _pending.pop_front();
        }
        if (_predicted) {
            _predicted = advance(*_predicted, input, dt);
        }
        _input = input;
        return sequence;
    }

    void Prediction::reconcile(const rtype::protocol::OwnState &state)
    {
        const std::optional<gameLogic::Vec2Pos> before = _predicted;
        gameLogic::Vec2Pos pos = state.pos;

        _inverted = state.inverted;
        _speedFactor = static_cast<float>(state.speedPercent)
            / rtype::config::SPEED_PERCENT;
        while (!_pending.empty() && _pending.front().sequence <= state.lastInput) {
            _pending.pop_front();
        }
        for (const Pending &pending : _pending) {
            pos = advance(pos, pending.input, pending.dt);
        }
        _predicted = pos;
        if (!before) {
            return;
        }
        _offset = _offset + gameLogic::Vec2{before->x - pos.x, before->y - pos.y};
        if (gameLogic::length(_offset) > SNAP_DISTANCE) {
            _offset = {};
        }
    }

    void Prediction::update(const float dt)
    {
        _offset = _offset * std::exp(-SMOOTHING_RATE * dt);
    }

    void Prediction::reset()
    {
        _predicted.reset();
        _offset = {};
    }

    std::optional<gameLogic::Vec2Pos> Prediction::position() const
    {
        if (!_predicted) {
            return std::nullopt;
        }
        return gameLogic::Vec2Pos{*_predicted + _offset};
    }

    gameLogic::Vec2Velocity Prediction::velocity() const
    {
        return gameLogic::Vec2Velocity{rtype::inputDirection(_input, _inverted)
            * (rtype::config::PLAYER_SPEED * _speedFactor)};
    }

    void Prediction::setWorld(const gameLogic::Vec2Hitbox &box,
        std::vector<rtype::Collider> solids)
    {
        _box = box;
        _solids = std::move(solids);
    }

    bool Prediction::inverted() const
    {
        return _inverted;
    }

    float Prediction::speedFactor() const
    {
        return _speedFactor;
    }

    gameLogic::Vec2Pos Prediction::advance(const gameLogic::Vec2Pos &from,
        const rtype::PlayerInput &input, const float dt) const
    {
        const gameLogic::Vec2 step = rtype::inputDirection(input, _inverted)
            * (rtype::config::PLAYER_SPEED * _speedFactor * dt);
        const rtype::Collider moved{{}, gameLogic::Vec2Pos{from + step}, _box,
            {}};

        return rtype::resolveSolids(moved, _solids);
    }
}
