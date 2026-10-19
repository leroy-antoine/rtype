#pragma once

#include <optional>
#include <vector>
#include <cstddef>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include "engine/audio/AudioPlayer.hpp"

namespace Engine {
    struct AudioPlayer::Impl {
        explicit Impl(std::size_t voiceCount) : voices(voiceCount) {}

        std::vector<std::optional<sf::Sound>> voices;
        std::size_t next = 0;
        sf::Music music;
        float soundVolume = 1.0f;
        float musicVolume = 1.0f;
    };
}