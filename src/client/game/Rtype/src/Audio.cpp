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

        constexpr std::array<const char *, 2> SHOP_FILES = {
            "trade_accept.ogg", "trade_refuse.ogg"};
    }

    Audio::Audio(const std::filesystem::path &assets) : _dir(assets / "sounds")
    {
        try {
            load();
            loadShopSounds();
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

    void Audio::loadShopSounds()
    {
        for (std::size_t i = 0; i < SHOP_SOUND_COUNT; ++i) {
            try {
                _shopIds[i] = _sounds.load((_dir / SHOP_FILES[i]).string());
            } catch (const std::exception &e) {
                std::cerr << "R-Type: shop sound missing (" << e.what() << ")"
                    << std::endl;
            }
        }
    }

    void Audio::playShopSound(const ShopSound sound, const float volume)
    {
        const std::optional<Engine::SoundID> &id =
            _shopIds[static_cast<std::size_t>(sound)];

        if (_enabled && id) {
            _player.play(*id, volume);
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
            } else if (track == Track::Level) {
                _player.playMusic((_dir / "music_level.ogg").string());
            } else {
                _player.playMusic((_dir / "music_boss.ogg").string());
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
            case Cue::Purchase: playShopSound(ShopSound::Accept, 0.8f); break;
            case Cue::PurchaseRefused: playShopSound(ShopSound::Refuse, 0.8f); break;
            case Cue::BossAppears:
                playSound(Sound::BossAlert, 0.9f);
                playMusic(Track::Boss);
                break;
        }
    }

    void Audio::follow(const Replica &replica)
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
        if (status->bossMaxHp > 0) {
            playMusic(Track::Boss);
        } else {
            playMusic(Track::Level);
        }
    }

    void Audio::onPhase(const rtype::Phase phase)
    {
        _phase = phase;
        if (phase == rtype::Phase::Playing) {
            return;
        }
        playMusic(Track::None);
        if (phase == rtype::Phase::Victory) {
            playSound(Sound::Victory, 0.9f);
        } else {
            playSound(Sound::Defeat, 0.9f);
        }
    }
}
