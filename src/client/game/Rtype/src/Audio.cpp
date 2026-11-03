#include <exception>
#include <iostream>
#include "client/Audio.hpp"

namespace client {
    namespace {
        constexpr float SOUND_VOLUME = 0.6f;
        constexpr float MUSIC_VOLUME = 0.35f;
        constexpr float SHOT_VOLUME = 0.25f;

        constexpr std::array<const char *, 7> FILES = {
            "shoot.wav", "hit.wav", "explosion.wav", "boss_alert.wav",
            "victory.wav", "defeat.wav", "coin_pickup.mp3"};

        constexpr std::array<const char *, 10> MUSIC = {
            "", "music_level.ogg", "music_boss.ogg", "dasher_theme.ogg",
            "summoner_theme.ogg", "music_boss_fortress.ogg",
            "music_deathrun.ogg", "music_nebula.ogg", "music_wreckage.ogg",
            "music_core.ogg"};
    }

    Audio::Audio(const std::filesystem::path &assets) : _dir(assets / "sounds")
    {
        try {
            load();
            _player.setMusicVolume(MUSIC_VOLUME);
            _player.setSoundVolume(SOUND_VOLUME);
            _enabled = true;
        } catch (const std::exception &e) {
            std::cerr << "R-Type: sound disabled (" << e.what() << ")"
                << std::endl;
        }
    }

    void Audio::load()
    {
        for (std::size_t i = 0; i < SOUND_COUNT; ++i) {
            _ids[i] = _sounds.load((_dir / FILES[i]).string());
        }
    }

    void Audio::playSound(const Sound sound, const float volume)
    {
        if (_enabled) {
            _player.play(_ids[static_cast<std::size_t>(sound)], volume);
        }
    }

    void Audio::playMusic(const Track track)
    {
        if (!_enabled || track == _track) {
            return;
        }
        _track = track;
        try {
            if (track == Track::None) {
                _player.stopMusic();
            } else {
                _player.playMusic(
                    (_dir / MUSIC[static_cast<std::size_t>(track)]).string());
            }
        } catch (const std::exception &e) {
            std::cerr << "R-Type: music disabled (" << e.what() << ")"
                << std::endl;
            _enabled = false;
        }
    }

    // Un seul son de tir par image, meme si plusieurs balles apparaissent.
    void Audio::play(const std::vector<Cue> &cues)
    {
        bool shot = false;

        for (const Cue cue : cues) {
            if (cue == Cue::PlayerShot && shot) {
                continue;
            }
            shot = shot || cue == Cue::PlayerShot;
            playCue(cue);
        }
    }

    void Audio::playCue(const Cue cue)
    {
        switch (cue) {
            case Cue::PlayerShot: playSound(Sound::Shoot, SHOT_VOLUME); break;
            case Cue::Hit: playSound(Sound::Hit, 0.35f); break;
            case Cue::Explosion: playSound(Sound::Explosion, 0.7f); break;
            case Cue::BossExplosion: playSound(Sound::Explosion, 1.f); break;
            case Cue::CoinPickup: playSound(Sound::Coin, 0.6f); break;
            case Cue::BossAppears:
                playSound(Sound::BossAlert, 0.9f);
                break;
        }
    }

    void Audio::follow(Replica &replica)
    {
        const std::optional<rtype::GameStatus> &status = replica.status();

        if (!status) {
            return;
        }
        if (status->phase != _phase) {
            onPhase(status->phase);
        }
        if (status->phase != rtype::Phase::Playing) {
            return;
        }
        playMusic(trackFor(replica));
    }

    Audio::Track Audio::trackOfZone(const rtype::Zone zone)
    {
        switch (zone) {
            case rtype::Zone::Warp: return Track::DeathRun;
            case rtype::Zone::Nebula: return Track::Nebula;
            case rtype::Zone::Wreckage: return Track::Wreckage;
            case rtype::Zone::Core: return Track::Core;
            default: return Track::Level;
        }
    }

    Audio::Track Audio::trackFor(Replica &replica)
    {
        switch (replica.bossSprite().value_or(rtype::SpriteId::Count)) {
            case rtype::SpriteId::Boss: return Track::BossStandard;
            case rtype::SpriteId::DasherBoss: return Track::BossDasher;
            case rtype::SpriteId::SummonerBoss: return Track::BossSummoner;
            case rtype::SpriteId::FortressBoss: return Track::BossFortress;
            default: break;
        }
        const auto &status = replica.status();

        return trackOfZone(rtype::zoneOf(status->map, status->level));
    }

    void Audio::onPhase(const rtype::Phase phase)
    {
        _phase = phase;
        if (phase == rtype::Phase::Playing) {
            return;
        }
        playMusic(Track::None);
        if (phase == rtype::Phase::Lobby) {
            return;
        }
        if (phase == rtype::Phase::Victory) {
            playSound(Sound::Victory, 0.9f);
        } else {
            playSound(Sound::Defeat, 0.9f);
        }
    }
}
