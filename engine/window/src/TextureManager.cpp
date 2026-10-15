#include <stdexcept>
#include <memory>
#include "engine/window/TextureManager.hpp"
#include "engine/window/TextureManagerImpl.hpp"

namespace Engine {

    TextureManager::TextureManager() : _impl(std::make_unique<Impl>()) {}
    TextureManager::~TextureManager() = default;

    TextureID TextureManager::load(const std::string& fileName) {
        if (const auto it = _impl->ids.find(fileName); it != _impl->ids.end())
            return it->second;
        sf::Texture texture;
        if (!texture.loadFromFile(fileName))
            throw std::runtime_error("TextureManager: failed to load file: " + fileName);

        const auto id = static_cast<TextureID>(_impl->textures.size());
        _impl->textures.push_back(std::move(texture));
        _impl->ids.emplace(fileName, id);
        return id;
    }
}
