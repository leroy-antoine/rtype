#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

namespace server {
    struct Command {
        enum class Kind { Help, Rooms, Players, Kick, Close, Quit };

        Kind kind = Kind::Help;
        std::uint32_t target = 0;
    };

    std::variant<Command, std::string> parseCommand(std::string_view line);
}
