#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <vector>
#include "logic/Colisions.hpp"
#include "logic/Components.hpp"
#include "rtype/Protocol.hpp"
#include "utils/Vectors.hpp"

namespace client {
    class Prediction {
    public:
        std::uint32_t record(const rtype::PlayerInput &input, float dt);
        void reconcile(const rtype::protocol::OwnState &state);
        void update(float dt);
        void reset();
        void setWorld(const gameLogic::Vec2Hitbox &box,
            std::vector<rtype::Collider> solids);
        [[nodiscard]] bool inverted() const;
        [[nodiscard]] float speedFactor() const;

        [[nodiscard]] std::optional<gameLogic::Vec2Pos> position() const;
        [[nodiscard]] gameLogic::Vec2Velocity velocity() const;

    private:
        static constexpr std::size_t MAX_PENDING = 120;
        static constexpr float SNAP_DISTANCE = 150.f;
        static constexpr float SMOOTHING_RATE = 15.f;

        struct Pending {
            std::uint32_t sequence;
            rtype::PlayerInput input;
            float dt;
        };

        [[nodiscard]] gameLogic::Vec2Pos advance(const gameLogic::Vec2Pos &from,
            const rtype::PlayerInput &input, float dt) const;

        std::uint32_t _nextSequence = 1;
        std::deque<Pending> _pending;
        std::optional<gameLogic::Vec2Pos> _predicted;
        gameLogic::Vec2 _offset{};
        rtype::PlayerInput _input;
        gameLogic::Vec2Hitbox _box{};
        std::vector<rtype::Collider> _solids;
        bool _inverted = false;
        float _speedFactor = 1.f;
    };
}
