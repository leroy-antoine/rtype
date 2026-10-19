#include <exception>
#include <filesystem>
#include <iostream>
#include "rtype/ReturnCodes.hpp"
#include "game/App.hpp"

int main(int argc, char **argv)
{
    const std::filesystem::path binary = argc > 0 ? argv[0] : "";

    try {
        client::App app(binary.parent_path() / "assets");

        return app.run();
    } catch (const std::exception &e) {
        std::cerr << "R-Type: " << e.what() << std::endl;
        return RTYPE_ERROR;
    }
}
