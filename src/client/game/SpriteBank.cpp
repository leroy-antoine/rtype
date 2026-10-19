#include "SpriteBank.hpp"

namespace client {
    SpriteBank::SpriteBank(Engine::TextureManager &textures, const std::filesystem::path &assets)
    {
        const auto load = [&](const char *file) {
            return textures.load((assets / file).string());
        };
        const auto set = [this](rtype::SpriteId id, const Engine::Sprite &sprite) {
            _sprites[static_cast<std::size_t>(id)] = sprite;
        };

        set(rtype::SpriteId::Player, {load("r-typesheet42.gif"),
            {.firstFrame = {0, 0, 33, 17}, .frameCount = 5}});
        set(rtype::SpriteId::Monster, {load("r-typesheet5.gif"),
            {.firstFrame = {0, 0, 33, 36}, .frameCount = 8, .frameDuration = 0.08f}});
        set(rtype::SpriteId::PlayerBullet, {load("r-typesheet1.gif"),
            {.firstFrame = {249, 90, 16, 4}, .frameCount = 2, .step = 18}});
        set(rtype::SpriteId::MonsterBullet, {load("r-typesheet1.gif"),
            {.firstFrame = {226, 276, 16, 11}, .frameCount = 2, .step = 16, .frameDuration = 0.15f}});
        set(rtype::SpriteId::Explosion, {load("r-typesheet44.gif"),
            {.firstFrame = {129, 0, 33, 33}, .frameCount = 6}});
    }

    const Engine::Sprite &SpriteBank::get(const rtype::SpriteId id) const
    {
        return _sprites.at(static_cast<std::size_t>(id));
    }
}
