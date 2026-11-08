#include <array>
#include <charconv>
#include <limits>
#include <optional>
#include <sstream>
#include "Command.hpp"

namespace server {
    namespace {
        struct Entry {
            std::string_view name;
            Command::Kind kind;
            std::uint32_t maxTarget;
        };

        constexpr std::array<Entry, 6> COMMANDS{{
            {"help", Command::Kind::Help, 0},
            {"rooms", Command::Kind::Rooms, 0},
            {"players", Command::Kind::Players, 0},
            {"kick", Command::Kind::Kick, std::numeric_limits<std::uint32_t>::max()},
            {"close", Command::Kind::Close, std::numeric_limits<std::uint16_t>::max()},
            {"quit", Command::Kind::Quit, 0},
        }};

        std::optional<std::uint32_t> parseNumber(const std::string &text, const std::uint32_t max)
        {
            std::uint32_t value = 0;
            const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);

            if (error != std::errc{} || end != text.data() + text.size() || value > max) {
                return std::nullopt;
            }
            return value;
        }
    }

    std::variant<Command, std::string> parseCommand(const std::string_view line)
    {
        std::istringstream words{std::string(line)};
        std::string name;
        std::string argument;
        std::string extra;

        words >> name >> argument >> extra;
        if (name.empty() || name.front() != '/') {
            return std::string("commands start with '/', type /help");
        }
        name.erase(0, 1);
        for (const Entry &entry : COMMANDS) {
            if (entry.name != name) {
                continue;
            }
            const std::string usage = "usage: /" + name + (entry.maxTarget ? (name == "kick" ? " <client>" : " <room>") : "");

            if (!extra.empty() || argument.empty() != (entry.maxTarget == 0)) {
                return usage;
            }
            if (entry.maxTarget == 0) {
                return Command{entry.kind, 0};
            }
            const std::optional<std::uint32_t> target = parseNumber(argument, entry.maxTarget);

            if (!target) {
                return usage;
            }
            return Command{entry.kind, *target};
        }
        return "unknown command /" + name + ", type /help";
    }
}
