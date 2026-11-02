#include "client/SpriteBank.hpp"

namespace client {
    using Id = rtype::SpriteId;

    SpriteBank::SpriteBank(Engine::TextureManager &textures,
        const std::filesystem::path &assets)
        : _textures(textures), _assets(assets)
    {
        loadMonsters();
        loadMoreMonsters();
        loadBosses();
        loadShots();
        loadMisc();
    }

    Engine::TextureID SpriteBank::texture(const char *file)
    {
        return _textures.load((_assets / file).string());
    }

    void SpriteBank::add(const Id id, const char *file,
        const Engine::Animation &animation)
    {
        _sprites[static_cast<std::size_t>(id)] = {texture(file), animation};
    }

    void SpriteBank::loadMonsters()
    {
        add(Id::Monster, "r-typesheet5.gif",
            {.firstFrame = {0, 0, 33, 36}, .frameCount = 8,
                .frameDuration = 0.08f});
        add(Id::Tank, "r-typesheet26.gif",
            {.firstFrame = {0, 0, 65, 50}, .frameCount = 6,
                .frameDuration = 0.12f});
        add(Id::Kamikaze, "r-typesheet16.gif",
            {.firstFrame = {0, 12, 17, 11}, .frameCount = 4, .step = 17,
                .frameDuration = 0.06f});
    }

    void SpriteBank::loadMoreMonsters()
    {
        add(Id::Elite, "r-typesheet19.gif",
            {.firstFrame = {0, 0, 32, 33}, .frameCount = 7, .step = 33});
        add(Id::Rocket, "r-typesheet11.gif",
            {.firstFrame = {0, 0, 33, 34}, .frameCount = 6,
                .frameDuration = 0.15f});
        add(Id::Bomber, "r-typesheet17.gif",
            {.firstFrame = {0, 0, 65, 132}, .frameCount = 8,
                .frameDuration = 0.15f});
    }

    void SpriteBank::loadBosses()
    {
        add(Id::Boss, "r-typesheet32.gif",
            {.firstFrame = {0, 430, 260, 143}, .frameCount = 2, .step = 260,
                .frameDuration = 0.35f});
        add(Id::DasherBoss, "r-typesheet21.gif",
            {.firstFrame = {25, 138, 65, 50}, .frameCount = 7, .step = 65,
                .frameDuration = 0.07f});
        add(Id::SummonerBoss, "r-typesheet40.gif",
            {.firstFrame = {0, 0, 66, 60}, .frameCount = 8,
                .frameDuration = 0.2f});
        add(Id::FortressBoss, "r-typesheet37.gif",
            {.firstFrame = {0, 0, 594, 178}, .frameCount = 1});
    }

    void SpriteBank::loadShots()
    {
        add(Id::PlayerBullet, "r-typesheet1.gif",
            {.firstFrame = {249, 90, 16, 4}, .frameCount = 2, .step = 18});
        add(Id::MonsterBullet, "r-typesheet1.gif",
            {.firstFrame = {226, 276, 16, 11}, .frameCount = 2, .step = 16,
                .frameDuration = 0.15f});
        add(Id::RocketMissile, "r-typesheet10.gif",
            {.firstFrame = {148, 78, 15, 6}, .frameCount = 1});
        add(Id::Bomb, "r-typesheet3.gif",
            {.firstFrame = {137, 2, 17, 14}, .frameCount = 4, .step = 17,
                .frameDuration = 0.12f});
        add(Id::AlienShard, "r-typesheet9.gif",
            {.firstFrame = {202, 80, 26, 9}, .frameCount = 1});
    }

    void SpriteBank::loadMisc()
    {
        add(Id::Player, "r-typesheet42.gif",
            {.firstFrame = {0, 0, 33, 17}, .frameCount = 5});
        add(Id::Explosion, "r-typesheet44.gif",
            {.firstFrame = {129, 0, 33, 33}, .frameCount = 6});
        add(Id::Coin, "coin_spin_sheet.png",
            {.firstFrame = {0, 0, 128, 128}, .frameCount = 6});
        add(Id::RockWall, "obstacle_rock.png",
            {.firstFrame = {0, 0, 32, 32}, .frameCount = 1});
        add(Id::HazardWall, "obstacle_hazard.png",
            {.firstFrame = {0, 0, 32, 32}, .frameCount = 1});
        _clock = {texture("stopwatch_sheet.png"),
            {.firstFrame = {0, 0, 32, 32}, .frameCount = 16}};
    }

    const Engine::Sprite &SpriteBank::get(const Id id) const
    {
        return _sprites.at(static_cast<std::size_t>(id));
    }

    const Engine::Sprite &SpriteBank::clock() const
    {
        return _clock;
    }
}
