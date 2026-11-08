#include <string>
#include <variant>
#include "admin/Command.hpp"
#include "TestHarness.hpp"

using server::Command;
using server::parseCommand;

namespace {
    bool parsesTo(const std::string &line, const Command::Kind kind, const std::uint32_t target = 0)
    {
        const auto parsed = parseCommand(line);
        const Command *command = std::get_if<Command>(&parsed);

        return command && command->kind == kind && command->target == target;
    }

    bool isError(const std::string &line)
    {
        return std::holds_alternative<std::string>(parseCommand(line));
    }
}

TEST(commands_without_argument_are_parsed)
{
    CHECK(parsesTo("/help", Command::Kind::Help));
    CHECK(parsesTo("/rooms", Command::Kind::Rooms));
    CHECK(parsesTo("/players", Command::Kind::Players));
    CHECK(parsesTo("/quit", Command::Kind::Quit));
    CHECK(parsesTo("   /rooms   ", Command::Kind::Rooms));
}

TEST(commands_with_a_target_are_parsed)
{
    CHECK(parsesTo("/kick 3", Command::Kind::Kick, 3));
    CHECK(parsesTo("/close 12", Command::Kind::Close, 12));
    CHECK(parsesTo("/kick 4294967295", Command::Kind::Kick, 4294967295u));
    CHECK(parsesTo("/close 65535", Command::Kind::Close, 65535));
}

TEST(a_line_without_slash_is_refused)
{
    CHECK(isError(""));
    CHECK(isError("rooms"));
    CHECK(isError("kick 3"));
    CHECK(std::get<std::string>(parseCommand("rooms")).find("/help") != std::string::npos);
}

TEST(an_unknown_command_is_refused)
{
    CHECK(isError("/"));
    CHECK(isError("/ban 3"));
    CHECK(isError("/ROOMS"));
    CHECK(std::get<std::string>(parseCommand("/ban 3")).find("unknown command /ban") != std::string::npos);
}

TEST(a_bad_argument_shows_the_usage)
{
    CHECK(isError("/kick"));
    CHECK(isError("/kick abc"));
    CHECK(isError("/kick -1"));
    CHECK(isError("/kick 3x"));
    CHECK(isError("/kick 3 4"));
    CHECK(isError("/kick 4294967296"));
    CHECK(isError("/close 65536"));
    CHECK(isError("/rooms 1"));
    CHECK(std::get<std::string>(parseCommand("/kick")) == "usage: /kick <client>");
    CHECK(std::get<std::string>(parseCommand("/close x")) == "usage: /close <room>");
    CHECK(std::get<std::string>(parseCommand("/quit now")) == "usage: /quit");
}
