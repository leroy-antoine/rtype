#pragma once

#include <memory>
#include <string>

#include "engine/audio/Types.hpp"
#include "engine/audio/Export.hpp"

namespace Engine {
    class ENGINE_AUDIO_EXPORT SoundManager {
        public:
            SoundManager();
            ~SoundManager();

            SoundManager(const SoundManager&) = delete;
            SoundManager& operator=(const SoundManager&) = delete;

            SoundID load(const std::string & path);

        private:
            friend class AudioPlayer;
            struct Impl;
            std::unique_ptr<Impl> _impl;
    };
}