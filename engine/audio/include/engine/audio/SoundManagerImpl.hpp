#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <SFML/Audio/SoundBuffer.hpp>
#include "engine/audio/SoundManager.hpp"

namespace Engine {
    struct SoundManager::Impl {
        std::vector<std::unique_ptr<sf::SoundBuffer>> buffers;
        std::unordered_map<std::string, SoundID> ids;
    };
}