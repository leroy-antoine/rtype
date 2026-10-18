#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include "engine/window/TextureManager.hpp"

namespace Engine {
    struct TextureManager::Impl {
        std::vector<sf::Texture> textures;
        std::unordered_map<std::string, TextureID> ids;
    };
}