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
#include "CustomMapList.hpp"
#include "GameView.hpp"
#include "LobbyScreen.hpp"
#include "Replica.hpp"
#include "ShopPanel.hpp"

namespace client {
    class App {
    public:
        App(std::filesystem::path assets, std::filesystem::path customMaps,
            const std::string &host, std::uint16_t port);

        int run();

    private:
        using Clock = std::chrono::steady_clock;

        static constexpr unsigned int WINDOW_WIDTH = 1280;
        static constexpr unsigned int WINDOW_HEIGHT = 720;
        static constexpr unsigned int FRAMERATE = 60;
        static constexpr float WORLD_TO_SCREEN = static_cast<float>(WINDOW_WIDTH) / rtype::config::MAP_WIDTH;

        // Les sons du monde recu a la connexion ne sont pas rejoues.
        static constexpr float LISTEN_DELAY = 0.5f;

        bool loop(Engine::Window &window, GameView &view, Audio &audio, ShopPanel &shop, LobbyScreen &lobby);
        void playRoom(float dt, GameView &view, Audio &audio, ShopPanel &shop);
        void browseRooms(LobbyScreen &lobby);
        void leaveRoom(Audio &audio);
        void playSounds(Audio &audio);
        bool listening();
        void sendControls(float dt, ShopPanel &shop);
        void useShop(ShopPanel &shop);
        [[nodiscard]] bool inLobby() const;
        [[nodiscard]] bool isPaused() const;
        void useMenu();
        void useMapStep();
        [[nodiscard]] rtype::Settings currentSettings() const;
        void useSettingsStep();
        void useScoresStep();
        void useCustomStep();
        void useCustomScoresPage(bool wasCustom);
        void startCustomMap();
        void requestShownRecords();
        [[nodiscard]] bool pressed(Engine::Key key) const;
        void handleBack(Engine::Window &window, Audio &audio);
        void useMatch();
        void usePause();
        void sendReady();
        [[nodiscard]] bool isReady(const rtype::GameStatus &status) const;
        float advance();
        [[nodiscard]] float elapsed() const;

        std::filesystem::path _assets;
        Network _network;
        Replica _replica;
        CustomMapList _custom;
        Controls _controls;
        Clock::time_point _start;
        Clock::time_point _last;
        std::optional<float> _connectedSince;
    };
}
