#include <exception>
#include <filesystem>
#include <iostream>
#include "rtype/ReturnCodes.hpp"
#include "game/App.hpp"

int main(const int, char **argv)
{
    try {
        client::App app(std::filesystem::path(argv[0]).parent_path() / "assets");

        return app.run();
    } catch (const std::exception &e) {
        std::cerr << "R-Type: " << e.what() << std::endl;
        return RTYPE_ERROR;
    }
}
