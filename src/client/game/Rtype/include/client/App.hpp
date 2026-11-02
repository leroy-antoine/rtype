#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include "engine/window/Window.hpp"
#include "network/Network.hpp"
#include "Audio.hpp"
#include "logic/Config.hpp"
#include "Controls.hpp"
#include "GameView.hpp"
#include "Replica.hpp"

namespace client {
    class App {
    public:
        App(std::filesystem::path assets, const std::string &host, std::uint16_t port);

        int run();

    private:
        using Clock = std::chrono::steady_clock;

        static constexpr unsigned int WINDOW_WIDTH = 1280;
        static constexpr unsigned int WINDOW_HEIGHT = 720;
        static constexpr unsigned int FRAMERATE = 60;
        static constexpr float WORLD_TO_SCREEN = static_cast<float>(WINDOW_WIDTH) / rtype::config::MAP_WIDTH;

        // Les sons du monde recu a la connexion ne sont pas rejoues.
        static constexpr float LISTEN_DELAY = 0.5f;

        bool loop(Engine::Window &window, GameView &view, Audio &audio);
        void playSounds(Audio &audio);
        bool listening();
        void sendControls(float dt);
        [[nodiscard]] bool inLobby() const;
        void useMenu();
        float advance();
        [[nodiscard]] float elapsed() const;

        std::filesystem::path _assets;
        Network _network;
        Replica _replica;
        Controls _controls;
        Clock::time_point _start;
        Clock::time_point _last;
        std::optional<float> _connectedSince;
    };
}
