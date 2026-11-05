#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>
#include "engine/audio/AudioPlayer.hpp"
#include "engine/audio/SoundManager.hpp"
#include "Replica.hpp"
#include "logic/Status.hpp"
#include "logic/Zones.hpp"

namespace client {
    // Sons et musique. Si les fichiers ou la carte son manquent, le jeu
    // continue sans bruit.
    class Audio {
    public:
        explicit Audio(const std::filesystem::path &assets);

        void play(const std::vector<Cue> &cues);
        // Suit l'etat de la partie : musique du niveau ou du boss, jingles de fin.
        void follow(Replica &replica);

    private:
        enum class Sound { Shoot, Hit, Explosion, BossAlert, Victory, Defeat,
            Coin, Count };
        enum class Track { None, Level, BossStandard, BossDasher,
            BossSummoner, BossFortress, DeathRun, Nebula, Wreckage, Core,
            Count };
        enum class ShopSound { Accept, Refuse, Count };

        static constexpr std::size_t SOUND_COUNT =
            static_cast<std::size_t>(Sound::Count);
        static constexpr std::size_t SHOP_SOUND_COUNT =
            static_cast<std::size_t>(ShopSound::Count);

        void load();
        void loadShopSounds();
        void playShopSound(ShopSound sound, float volume);
        void playSound(Sound sound, float volume);
        void playMusic(Track track);
        void playCue(Cue cue);
        void onPhase(rtype::Phase phase);
        [[nodiscard]] static Track trackOfZone(rtype::Zone zone);
        [[nodiscard]] static Track trackFor(Replica &replica);

        std::filesystem::path _dir;
        Engine::SoundManager _sounds;
        Engine::AudioPlayer _player{_sounds};
        std::array<Engine::SoundID, SOUND_COUNT> _ids{};
        std::array<std::optional<Engine::SoundID>, SHOP_SOUND_COUNT> _shopIds{};
        bool _enabled = false;
        Track _track = Track::None;
        rtype::Phase _phase = rtype::Phase::Playing;
    };
}
