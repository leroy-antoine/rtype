#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <vector>
#include "engine/audio/AudioPlayer.hpp"
#include "engine/audio/SoundManager.hpp"
#include "Replica.hpp"
#include "logic/Status.hpp"

namespace client {
    // Sons et musique. Si les fichiers ou la carte son manquent, le jeu
    // continue sans bruit.
    class Audio {
    public:
        explicit Audio(const std::filesystem::path &assets);

        void play(const std::vector<Cue> &cues);
        // Suit l'etat de la partie : musique du niveau ou du boss, jingles de fin.
        void follow(const Replica &replica);

    private:
        enum class Sound { Shoot, Hit, Explosion, BossAlert, Victory, Defeat, Count };
        enum class Track { None, Level, Boss };

        static constexpr std::size_t SOUND_COUNT =
            static_cast<std::size_t>(Sound::Count);

        void load();
        void playSound(Sound sound, float volume);
        void playMusic(Track track);
        void playCue(Cue cue);
        void onPhase(rtype::Phase phase);

        std::filesystem::path _dir;
        Engine::SoundManager _sounds;
        Engine::AudioPlayer _player{_sounds};
        std::array<Engine::SoundID, SOUND_COUNT> _ids{};
        bool _enabled = false;
        Track _track = Track::None;
        rtype::Phase _phase = rtype::Phase::Playing;
    };
}
