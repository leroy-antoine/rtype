#include <algorithm>
#include <stdexcept>
#include <SFML/Audio/Listener.hpp>
#include "engine/audio/AudioPlayer.hpp"
#include "engine/audio/AudioPlayerImpl.hpp"
#include "engine/audio/SoundManagerImpl.hpp"

namespace Engine {

    static float toSfmlVolume(float volume) {
        return std::clamp(volume, 0.0f, 1.0f) * 100.0f;
    }

    AudioPlayer::AudioPlayer(const SoundManager &sounds, std::size_t voices)
        : _sounds(sounds), _impl(std::make_unique<Impl>(voices)) {}
    AudioPlayer::~AudioPlayer() = default;

    void AudioPlayer::play(SoundID id, float volume) {
        const sf::SoundBuffer &buffer = *_sounds._impl->buffers.at(id);
        auto &voices = _impl->voices;

        auto it = std::find_if(voices.begin(), voices.end(), [](const auto &voice) {
            return !voice || voice->getStatus() == sf::Sound::Status::Stopped;
        });
        if (it == voices.end()) {
            it = voices.begin() + static_cast<std::ptrdiff_t>(_impl->next);
            _impl->next = (_impl->next + 1) % voices.size();
        }
        if (it == voices.end()) {
            it = voices.begin() + static_cast<std::ptrdiff_t>(_impl->next);
            _impl->next = (_impl->next + 1) % voices.size();
        }
        if (*it)
            (*it)->setBuffer(buffer);
        else
            it->emplace(buffer);
        (*it)->setVolume(toSfmlVolume(volume));
        (*it)->play();
    }

    void AudioPlayer::stopAllSounds() {
        for (auto &voice : _impl->voices) {
            if (voice) {
                voice->stop();
            }
        }
    }

    void AudioPlayer::playMusic(const std::string &filename, bool loop) {
        _impl->music.stop();
        if (!_impl->music.openFromFile(filename)) {
            throw std::runtime_error("AudioPlayer: Failed to load music from file: " + filename);
        }
        _impl->music.setLooping(loop);
        _impl->music.setVolume(toSfmlVolume(_impl->musicVolume));
        _impl->music.play();
    }

    void AudioPlayer::pauseMusic() {_impl->music.pause();}
    void AudioPlayer::resumeMusic() {_impl->music.play();}
    void AudioPlayer::stopMusic() {_impl->music.stop();}

    void AudioPlayer::setMasterVolume(float volume) {
        sf::Listener::setGlobalVolume(volume);
    }
    void AudioPlayer::setSoundVolume(float volume) {
        _impl->soundVolume = std::clamp(volume, 0.f, 1.f);
    }
    void AudioPlayer::setMusicVolume(float volume) {
        _impl->musicVolume = std::clamp(volume, 0.f, 1.f);
        _impl->music.setVolume(toSfmlVolume(_impl->musicVolume));

    }
}