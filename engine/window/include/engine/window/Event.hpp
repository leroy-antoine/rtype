#pragma once

#include <variant>
#include <cstdint>

namespace Engine {
    enum class Key {
        Unknown,
        Up,
        Down,
        Left,
        Right,
        Space,
        Enter,
        Escape,
        Tab,
        A,
        Z,
        E,
        R,
        Q,
        S,
        D,
        W,
        X,
        C
    };
    // a rajouter des keys pour la suite probablement esc ect ...

    namespace Events {
        struct Closed {};
        struct FocusLost {};
        struct FocusGained {};
        struct Resized {std::uint32_t width; std::uint32_t height;};
        struct KeyPressed {Key key;};
        struct KeyReleased {Key key;};
    }

    using Event = std::variant<
        Events::Closed,
        Events::FocusLost,
        Events::FocusGained,
        Events::Resized,
        Events::KeyPressed,
        Events::KeyReleased
    >;


}