#pragma once

#include <memory>
#include <string>
#include <cstddef>

#include "engine/audio/Types.hpp"
#include "engine/audio/SoundManager.hpp"
#include "engine/audio/Export.hpp"

namespace Engine {
    class ENGINE_AUDIO_EXPORT AudioPlayer {
        public:
            explicit AudioPlayer(const SoundManager &sounds, std::size_t voices = 32);
            ~AudioPlayer();

            AudioPlayer(const AudioPlayer&) = delete;
            AudioPlayer& operator=(const AudioPlayer&) = delete;

            void play(SoundID id, float volume = 1.0f);
            void stopAllSounds();

            void playMusic(const std::string &fileName, bool loop = true);
            void pauseMusic();
            void resumeMusic();
            void stopMusic();

            void setMasterVolume(float volume);
            void setSoundVolume(float volume);
            void setMusicVolume(float volume);

    private:
        const SoundManager &_sounds;
        struct Impl;
        std::unique_ptr<Impl> _impl;

    };
}