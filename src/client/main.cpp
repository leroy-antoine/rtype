#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "rtype/ReturnCodes.hpp"
#include "client/App.hpp"

static std::uint16_t parsePort(const std::string &text)
{
    const unsigned long port = std::stoul(text);

    if (port > std::numeric_limits<std::uint16_t>::max()) {
        throw std::out_of_range("port must be between 0 and 65535");
    }
    return static_cast<std::uint16_t>(port);
}

int main(const int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return RTYPE_ERROR;
    }
    try {
        client::App app(std::filesystem::path(argv[0]).parent_path() / "assets", argv[1], parsePort(argv[2]));

        return app.run();
    } catch (const std::exception &e) {
        std::cerr << "R-Type: " << e.what() << std::endl;
        return RTYPE_ERROR;
    }
}
