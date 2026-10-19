#include <stdexcept>
#include <memory>
#include "engine/audio/SoundManager.hpp"
#include "engine/audio/SoundManagerImpl.hpp"

namespace Engine {

    SoundManager::SoundManager() : _impl(std::make_unique<Impl>()) {}
    SoundManager::~SoundManager() = default;

    SoundID SoundManager::load(const std::string &path) {
        if (const auto it = _impl->ids.find(path); it != _impl->ids.end()) {
            return it->second;
        }

        auto buffer = std::make_unique<sf::SoundBuffer>();
        if (!buffer->loadFromFile(path)) {
            throw std::runtime_error("Failed to load sound from path: " + path);
        }
        const auto id = static_cast<SoundID>(_impl->buffers.size());
        _impl->buffers.push_back(std::move(buffer));
        _impl->ids.emplace(path, id);
        return id;
    }
}
