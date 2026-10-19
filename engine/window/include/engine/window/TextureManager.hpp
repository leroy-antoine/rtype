#pragma once

#include <memory>
#include <string>
#include "engine/window/Types.hpp"
#include "engine/window/Export.hpp"

namespace Engine {
    class ENGINE_WINDOW_EXPORT TextureManager {
        public:
        TextureManager();
        ~TextureManager();

        TextureManager(const TextureManager &) = delete;
        TextureManager &operator=(const TextureManager &) = delete;

        TextureID load(const std::string &fileName);

        private:
        friend class Renderer;
        struct Impl;
        std::unique_ptr<Impl> _impl;
    };

}