#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include "engine/window/Animation.hpp"
#include "engine/window/Sprite.hpp"
#include "engine/window/TextureManager.hpp"
#include "logic/Components.hpp"

namespace client {
    enum class BackdropId {
        NebulaBlue, NebulaPurple, NebulaRed, NebulaGreen, NebulaOrange,
        Rocks, Girders, Ring, Count
    };

    class SpriteBank {
    public:
        SpriteBank(Engine::TextureManager &textures,
            const std::filesystem::path &assets);

        [[nodiscard]] const Engine::Sprite &get(rtype::SpriteId id) const;
        [[nodiscard]] const Engine::Sprite &clock() const;
        [[nodiscard]] const Engine::Sprite &backdrop(BackdropId id) const;

    private:
        Engine::TextureID texture(const char *file);
        void add(rtype::SpriteId id, const char *file,
            const Engine::Animation &animation);
        void loadMonsters();
        void loadMoreMonsters();
        void loadBosses();
        void loadShots();
        void loadMisc();
        void loadObstacles();
        void loadBackdrops();
        void addBackdrop(BackdropId id, const char *file, int width,
            int height);

        Engine::TextureManager &_textures;
        std::filesystem::path _assets;
        std::array<Engine::Sprite,
            static_cast<std::size_t>(rtype::SpriteId::Count)> _sprites{};
        std::array<Engine::Sprite,
            static_cast<std::size_t>(BackdropId::Count)> _backdrops{};
        Engine::Sprite _clock{};
    };
}
