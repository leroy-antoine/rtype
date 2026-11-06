#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "client/LobbyMenu.hpp"
#include "utils/Wrap.hpp"
#include "TestHarness.hpp"

using namespace client;
using Engine::Key;

namespace {
    namespace fs = std::filesystem;

    const std::string GOOD =
        R"({"name":"Good","levels":[{"length":1000,)"
        R"("bosses":[{"kind":"Standard","at":900}]}]})";

    void write(const fs::path &file, const std::string &text)
    {
        std::ofstream(file, std::ios::binary) << text;
    }

    // a_bad.json (invalid) then b_good.json (valid).
    fs::path folder(const std::string &name)
    {
        const fs::path path = fs::temp_directory_path() / name;

        fs::remove_all(path);
        fs::create_directories(path);
        write(path / "a_bad.json", "{oops");
        write(path / "b_good.json", GOOD);
        return path;
    }
}

TEST(the_lobby_starts_on_the_maps_of_the_game)
{
    const fs::path path = folder("rtype_menu_start");
    const LobbyMenu menu(path);

    CHECK(menu.tab() == Tab::Maps);
    CHECK(menu.entries().size() == 2);
    CHECK(menu.selected() == nullptr);
    fs::remove_all(path);
}

TEST(tab_switches_between_the_maps_and_the_custom_maps)
{
    const fs::path path = folder("rtype_menu_tab");
    LobbyMenu menu(path);

    menu.navigate({Key::Tab});
    CHECK(menu.tab() == Tab::Custom);
    menu.navigate({Key::Tab});
    CHECK(menu.tab() == Tab::Maps);
    fs::remove_all(path);
}

TEST(the_leaderboard_is_only_opened_on_a_valid_map)
{
    const fs::path path = folder("rtype_menu_records");
    LobbyMenu menu(path);

    menu.navigate({Key::Tab});
    menu.navigate({Key::Right});
    CHECK(menu.tab() == Tab::Custom);
    CHECK(!menu.takeRecordsRequest().has_value());
    menu.moveCursor(1);
    menu.navigate({Key::Right});
    CHECK(menu.tab() == Tab::Records);
    CHECK(menu.takeRecordsRequest().has_value());
    CHECK(!menu.takeRecordsRequest().has_value());
    menu.navigate({Key::Left});
    CHECK(menu.tab() == Tab::Custom);
    fs::remove_all(path);
}

TEST(the_cursor_wraps_and_clears_the_message)
{
    const fs::path path = folder("rtype_menu_cursor");
    LobbyMenu menu(path);

    menu.setMessage("oops");
    menu.moveCursor(1);
    CHECK(menu.cursor() == 1);
    CHECK(menu.message().empty());
    menu.moveCursor(1);
    CHECK(menu.cursor() == 0);
    menu.moveCursor(-1);
    CHECK(menu.cursor() == 1);
    fs::remove_all(path);
}

TEST(the_records_follow_the_cursor_in_the_leaderboard)
{
    const fs::path path = folder("rtype_menu_follow");
    LobbyMenu menu(path);

    write(path / "c_good.json", GOOD + " ");
    menu.navigate({Key::Tab});
    menu.navigate({Key::R});
    menu.moveCursor(1);
    menu.navigate({Key::Right});
    CHECK(menu.takeRecordsRequest().has_value());
    menu.moveCursor(1);
    CHECK(menu.takeRecordsRequest().has_value());
    fs::remove_all(path);
}

TEST(only_a_valid_map_can_be_read_to_be_sent)
{
    const fs::path path = folder("rtype_menu_read");
    LobbyMenu menu(path);
    std::string text;

    CHECK(!menu.readSelected(text).empty());
    menu.moveCursor(1);
    CHECK(menu.readSelected(text).empty());
    CHECK(text == GOOD);
    fs::remove_all(path);
}

TEST(r_reads_the_folder_again)
{
    const fs::path path = folder("rtype_menu_refresh");
    LobbyMenu menu(path);

    write(path / "c.json", GOOD);
    CHECK(menu.entries().size() == 2);
    menu.navigate({Key::R});
    CHECK(menu.entries().size() == 2);
    menu.navigate({Key::Tab});
    menu.navigate({Key::R});
    CHECK(menu.entries().size() == 3);
    fs::remove_all(path);
}

TEST(an_empty_or_missing_folder_gives_an_empty_list)
{
    const fs::path path = fs::temp_directory_path() / "rtype_menu_missing";
    LobbyMenu menu(path);
    std::string text;

    fs::remove_all(path);
    menu.navigate({Key::Tab});
    menu.moveCursor(1);
    CHECK(menu.entries().empty());
    CHECK(menu.selected() == nullptr);
    CHECK(!menu.readSelected(text).empty());
    fs::remove_all(path);
}

TEST(a_long_text_is_cut_into_lines)
{
    const std::vector<std::string> lines = utils::wrap("aaa bbb ccc ddd", 7);

    CHECK(lines.size() == 2);
    CHECK(lines[0] == "aaa bbb");
    CHECK(lines[1] == "ccc ddd");
    CHECK(utils::wrap("short", 10).size() == 1);
    CHECK(utils::wrap("", 10).size() == 1);
    CHECK(utils::wrap("abcdefghij", 4).size() == 3);
    CHECK(utils::wrap("abc", 0).size() == 1);
}
