#include <csignal>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <variant>
#include "rtype/ReturnCodes.hpp"
#include "App.hpp"

namespace server {
    namespace {
        volatile std::sig_atomic_t running = 1;

        void stop(int)
        {
            running = 0;
        }
    }

    App::App(const std::uint16_t port)
        : _server(port, MAX_CLIENTS, rtype::protocol::VERSION), _lobby(_server), _admin(_server, _lobby, std::cout)
    {
    }

    int App::run()
    {
        std::signal(SIGINT, stop);
        std::signal(SIGTERM, stop);
        std::cout << "R-Type server listening on port " << _server.port() << ", type /help for the admin commands" << std::endl;
        while (running) {
            receive();
            runCommands();
            _lobby.sync();
            _server.flush();
            std::this_thread::sleep_for(POLL_DELAY);
        }
        return RTYPE_SUCCESS;
    }

    void App::runCommands()
    {
        for (const std::string &line : _console.take()) {
            if (!_admin.run(line)) {
                running = 0;
            }
        }
    }

    void App::receive()
    {
        _server.update();
        while (const std::optional<Engine::ServerEvent> event = _server.pollEvent()) {
            std::visit([this](const auto &e) { _lobby.handle(e); }, *event);
        }
    }
}