#include <cmath>
#include "logic/Config.hpp"
#include "logic/Movements.hpp"
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
            _predicted = gameLogic::Vec2Pos{*_predicted + step(input, dt)};
        }
        _input = input;
        return sequence;
    }

    void Prediction::reconcile(const rtype::protocol::OwnState &state)
    {
        const std::optional<gameLogic::Vec2Pos> before = _predicted;
        gameLogic::Vec2 pos = state.pos;

        while (!_pending.empty() && _pending.front().sequence <= state.lastInput) {
            _pending.pop_front();
        }
        for (const Pending &pending : _pending) {
            pos = pos + step(pending.input, pending.dt);
        }
        _predicted = gameLogic::Vec2Pos{pos};
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
        return gameLogic::Vec2Velocity{rtype::inputDirection(_input) * rtype::config::PLAYER_SPEED};
    }

    gameLogic::Vec2 Prediction::step(const rtype::PlayerInput &input, const float dt)
    {
        return rtype::inputDirection(input) * (rtype::config::PLAYER_SPEED * dt);
    }
}
