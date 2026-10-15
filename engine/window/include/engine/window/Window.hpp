#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <optional>
#include "engine/window/Event.hpp"
#include "engine/window/Export.hpp"

namespace Engine {
    class ENGINE_WINDOW_EXPORT Window {
        public:
        Window(std::uint32_t width, std::uint32_t height, const std::string &title);
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        bool isOpen() const;
        void close();
        void setFramerateLimit(unsigned int limit);
        std::optional<Event> pollEvent();
        void clear(std::uint8_t r, std::uint8_t g, std::uint8_t b);
        void display();

        private:
        friend class Renderer;
        struct Impl;
        std::unique_ptr<Impl> _impl;
    };
}