#include <array>
#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include "rtype/ReturnCodes.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/TextureManager.hpp"
#include "engine/window/Renderer.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Game.hpp"

namespace {
    constexpr unsigned int WINDOW_WIDTH = 1280;
    constexpr unsigned int WINDOW_HEIGHT = 720;
    constexpr float WORLD_TO_SCREEN = static_cast<float>(WINDOW_WIDTH) / rtype::config::MAP_WIDTH;
    constexpr int PLAYER_ROW_HEIGHT = 17;

    struct SpriteDef {
        Engine::TextureID texture = 0;
        Engine::IntRect rect;
    };

    using SpriteTable = std::array<SpriteDef, static_cast<std::size_t>(rtype::SpriteId::Count)>;

    SpriteTable loadSprites(Engine::TextureManager &textures, const std::filesystem::path &assets)
    {
        const auto load = [&](const char *file) {
            return textures.load((assets / file).string());
        };
        SpriteTable sprites{};

        sprites[static_cast<std::size_t>(rtype::SpriteId::Player)] =
            {load("r-typesheet42.gif"), {66, 0, 33, 17}};
        sprites[static_cast<std::size_t>(rtype::SpriteId::Monster)] =
            {load("r-typesheet5.gif"), {0, 0, 33, 36}};
        sprites[static_cast<std::size_t>(rtype::SpriteId::PlayerBullet)] =
            {load("r-typesheet1.gif"), {249, 90, 13, 4}};
        sprites[static_cast<std::size_t>(rtype::SpriteId::MonsterBullet)] =
            {load("r-typesheet1.gif"), {227, 278, 10, 6}};
        return sprites;
    }

    void applyKey(rtype::PlayerInput &input, const Engine::Key key, const bool pressed)
    {
        switch (key) {
            case Engine::Key::Up: input.up = pressed; break;
            case Engine::Key::Down: input.down = pressed; break;
            case Engine::Key::Left: input.left = pressed; break;
            case Engine::Key::Right: input.right = pressed; break;
            case Engine::Key::Space: input.fire = pressed; break;
            default: break;
        }
    }

    bool handleEvents(Engine::Window &window, rtype::PlayerInput &input)
    {
        bool respawn = false;

        while (const std::optional event = window.pollEvent()) {
            if (std::holds_alternative<Engine::Events::Closed>(*event)) {
                window.close();
            } else if (const auto *pressed = std::get_if<Engine::Events::KeyPressed>(&*event)) {
                if (pressed->key == Engine::Key::Escape) {
                    window.close();
                }
                respawn = respawn || pressed->key == Engine::Key::Enter;
                applyKey(input, pressed->key, true);
            } else if (const auto *released = std::get_if<Engine::Events::KeyReleased>(&*event)) {
                applyKey(input, released->key, false);
            }
        }
        return respawn;
    }

    void drawEntities(Engine::Renderer &renderer, Engine::Registry &reg, const SpriteTable &sprites)
    {
        reg.each<gameLogic::Vec2Pos, gameLogic::Vec2Hitbox, rtype::Renderable>(
            [&](const Engine::Entity e, const gameLogic::Vec2Pos &pos,
                const gameLogic::Vec2Hitbox &box, const rtype::Renderable &renderable) {
                const SpriteDef &def = sprites[static_cast<std::size_t>(renderable.id)];
                Engine::IntRect rect = def.rect;

                if (reg.has<rtype::PlayerTag>(e)) {
                    rect.top = reg.get<rtype::PlayerTag>(e).slot * PLAYER_ROW_HEIGHT;
                }
                renderer.draw(def.texture,
                    {pos.x * WORLD_TO_SCREEN, pos.y * WORLD_TO_SCREEN}, rect,
                    {box.x / static_cast<float>(rect.width) * WORLD_TO_SCREEN,
                     box.y / static_cast<float>(rect.height) * WORLD_TO_SCREEN});
            });
    }

    int run(const std::filesystem::path &assets)
    {
        Engine::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
        Engine::TextureManager textures;
        Engine::Renderer renderer(window, textures);
        const SpriteTable sprites = loadSprites(textures, assets);
        rtype::Game game;
        const std::optional<int> slot = game.addPlayer();
        rtype::PlayerInput input;

        using Clock = std::chrono::steady_clock;
        Clock::time_point last = Clock::now();
        float accumulator = 0.f;

        if (!slot) {
            std::cerr << "R-Type: no free player slot" << std::endl;
            return RTYPE_ERROR;
        }
        window.setFramerateLimit(60);
        while (window.isOpen()) {
            if (handleEvents(window, input) && !game.isPlayerAlive(*slot)) {
                game.removePlayer(*slot);
                game.addPlayer();
            }
            game.setInput(*slot, input);

            const Clock::time_point now = Clock::now();
            accumulator += std::chrono::duration<float>(now - last).count();
            last = now;
            while (accumulator >= rtype::config::FIXED_DT) {
                game.update(rtype::config::FIXED_DT);
                accumulator -= rtype::config::FIXED_DT;
            }

            window.clear(0, 0, 0);
            drawEntities(renderer, game.registry(), sprites);
            window.display();
        }
        return RTYPE_SUCCESS;
    }
}

int main(const int, char **argv)
{
    const std::filesystem::path assets =
        std::filesystem::path(argv[0]).parent_path() / "assets";

    try {
        return run(assets);
    } catch (const std::exception &e) {
        std::cerr << "R-Type: " << e.what() << std::endl;
        return RTYPE_ERROR;
    }
}
