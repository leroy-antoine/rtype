#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "client/CustomMapList.hpp"
#include "client/LobbyMenu.hpp"
#include "utils/Wrap.hpp"
#include "TestHarness.hpp"

using namespace client;

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

TEST(the_custom_list_holds_the_maps_of_the_folder)
{
    const fs::path path = folder("rtype_list_start");
    const CustomMapList list(path);

    CHECK(list.entries().size() == 2);
    CHECK(list.cursor() == 0);
    CHECK(list.selected() == nullptr);
    fs::remove_all(path);
}

TEST(the_cursor_wraps_and_a_real_move_clears_the_message)
{
    const fs::path path = folder("rtype_list_cursor");
    CustomMapList list(path);

    list.setMessage("oops");
    list.moveCursor(0);
    CHECK(list.message() == "oops");
    list.moveCursor(1);
    CHECK(list.cursor() == 1);
    CHECK(list.message().empty());
    CHECK(list.selected() != nullptr);
    list.moveCursor(1);
    CHECK(list.cursor() == 0);
    list.moveCursor(-1);
    CHECK(list.cursor() == 1);
    fs::remove_all(path);
}

TEST(only_a_valid_map_can_be_read_to_be_sent)
{
    const fs::path path = folder("rtype_list_read");
    CustomMapList list(path);
    std::string text;

    CHECK(!list.readSelected(text).empty());
    list.moveCursor(1);
    CHECK(list.readSelected(text).empty());
    CHECK(text == GOOD);
    fs::remove_all(path);
}

TEST(rescan_reads_the_folder_again)
{
    const fs::path path = folder("rtype_list_refresh");
    CustomMapList list(path);

    write(path / "c.json", GOOD);
    CHECK(list.entries().size() == 2);
    list.rescan();
    CHECK(list.entries().size() == 3);
    fs::remove_all(path);
}

TEST(an_empty_or_missing_folder_gives_an_empty_list)
{
    const fs::path path = fs::temp_directory_path() / "rtype_list_missing";
    CustomMapList list(path);
    std::string text;

    fs::remove_all(path);
    list.rescan();
    list.moveCursor(1);
    CHECK(list.entries().empty());
    CHECK(list.selected() == nullptr);
    CHECK(!list.readSelected(text).empty());
    fs::remove_all(path);
}

TEST(the_lobby_goes_back_one_screen_at_a_time)
{
    LobbyMenu menu;

    menu.openCustom();
    CHECK(menu.step() == LobbyStep::Custom);
    menu.openScores(true);
    CHECK(menu.step() == LobbyStep::Scores);
    menu.back();
    CHECK(menu.step() == LobbyStep::Custom);
    menu.back();
    CHECK(menu.step() == LobbyStep::Map);
}

TEST(the_scores_opened_from_the_game_list_go_back_to_it)
{
    LobbyMenu menu;

    menu.openScores();
    CHECK(menu.step() == LobbyStep::Scores);
    menu.back();
    CHECK(menu.step() == LobbyStep::Map);
}

TEST(the_scores_have_a_page_for_the_custom_maps_after_those_of_the_game)
{
    LobbyMenu menu;

    menu.openScores();
    for (std::size_t page = 0; page < rtype::MAP_COUNT; ++page) {
        CHECK(!menu.onCustomScores());
        menu.moveScoresMap(1);
    }
    CHECK(menu.onCustomScores());
    menu.moveScoresMap(1);
    CHECK(menu.scoresMap() == 0);
    menu.moveScoresMap(-1);
    CHECK(menu.onCustomScores());
}

TEST(the_scores_opened_from_the_custom_list_show_the_custom_page)
{
    LobbyMenu menu;

    menu.openScores(true);
    CHECK(menu.onCustomScores());
}

TEST(the_settings_of_a_custom_map_go_back_to_the_custom_list)
{
    LobbyMenu menu;

    menu.openCustom();
    menu.openSettings(rtype::Settings{}, true);
    CHECK(menu.settingsForCustom());
    menu.back();
    CHECK(menu.step() == LobbyStep::Custom);
    menu.openSettings(rtype::Settings{});
    CHECK(!menu.settingsForCustom());
    menu.back();
    CHECK(menu.step() == LobbyStep::Map);
}

TEST(reset_goes_back_to_the_first_screen)
{
    LobbyMenu menu;

    menu.openCustom();
    menu.openSettings(rtype::Settings{}, true);
    menu.reset();
    CHECK(menu.step() == LobbyStep::Map);
    CHECK(!menu.settingsForCustom());
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
