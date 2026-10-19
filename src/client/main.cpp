#include <exception>
#include <filesystem>
#include <iostream>
#include "rtype/ReturnCodes.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/TextureManager.hpp"
#include "engine/window/Renderer.hpp"
#include "engine/audio/SoundManager.hpp"
#include "engine/audio/AudioPlayer.hpp"
#include <algorithm>
#include <iostream>
#include <optional>
#include <variant>



struct AudioTest {
    Engine::AudioPlayer &player;
    Engine::SoundID sound;
    bool musicPaused = false;
    float musicVolume = 1.0f;
};

static void handleKey(AudioTest &audio, Engine::Key key)
{
    switch (key) {
        case Engine::Key::Space:
            audio.player.play(audio.sound);
            break;
        case Engine::Key::Enter:
            audio.musicPaused ? audio.player.resumeMusic() : audio.player.pauseMusic();
            audio.musicPaused = !audio.musicPaused;
            break;
        case Engine::Key::Up:
            audio.musicVolume = std::min(1.0f, audio.musicVolume + 0.1f);
            audio.player.setMusicVolume(audio.musicVolume);
            break;
        case Engine::Key::Down:
            audio.musicVolume = std::max(0.0f, audio.musicVolume - 0.1f);
            audio.player.setMusicVolume(audio.musicVolume);
            break;
        case Engine::Key::Escape:
            audio.player.stopAllSounds();
            break;
        default:
            break;
    }
}

static void handleEvents(Engine::Window &window, AudioTest &audio)
{
    while (const std::optional event = window.pollEvent()) {
        if (std::holds_alternative<Engine::Events::Closed>(*event))
            window.close();
        else if (const auto *pressed = std::get_if<Engine::Events::KeyPressed>(&*event))
            handleKey(audio, pressed->key);
    }
}

// Usage: r-type_client <sound.wav|ogg> [music.ogg]
// Space: play sound | Enter: pause/resume music | Up/Down: music volume | Escape: stop sounds
int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <sound> [music]\n";
        return 84;
    }

    Engine::Window window(800, 600, "Rtype client");
    Engine::TextureManager textures;
    Engine::Renderer renderer(window, textures);
    Engine::SoundManager sounds;
    Engine::AudioPlayer player(sounds);

    const Engine::TextureID ship = textures.load("sprites/r-typesheet42.gif");
    AudioTest audio{player, sounds.load(argv[1])};
    if (argc >= 3)
        player.playMusic(argv[2]);

    window.setFramerateLimit(60);
    while (window.isOpen()) {
        handleEvents(window, audio);
        window.clear(0, 0, 0);
        renderer.draw(ship, {100.f, 300.f}, {0, 0, 33, 17}, {3.f, 3.f});
        window.display();
    }
}

// static void test(sf::RenderWindow &window)
// {
//     while (const std::optional event = window.pollEvent()) {
//         if (event->is<sf::Event::Closed>())
//             window.close();
//     }
//     window.clear(sf::Color(20, 20, 40));
//     window.display();
// }
//
// int main()
// {
//     sf::RenderWindow window(sf::VideoMode({800u, 600u}), std::string("R-Type client"));
//     window.setFramerateLimit(60);
//
//     while (window.isOpen()) {
//         test(window);
//     }
//     return RTYPE_SUCCESS;
// }
