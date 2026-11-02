#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include "engine/window/Sprite.hpp"
#include "engine/window/TextureManager.hpp"
#include "logic/Components.hpp"

namespace client {
    class SpriteBank {
    public:
        SpriteBank(Engine::TextureManager &textures, const std::filesystem::path &assets);

        [[nodiscard]] const Engine::Sprite &get(rtype::SpriteId id) const;
        [[nodiscard]] const Engine::Sprite &clock() const;

    private:
        void set(rtype::SpriteId id, const Engine::Sprite &sprite);
        void loadIcons(Engine::TextureManager &textures,
            const std::filesystem::path &assets);

        std::array<Engine::Sprite, static_cast<std::size_t>(rtype::SpriteId::Count)> _sprites{};
        Engine::Sprite _clock{};
    };
}
