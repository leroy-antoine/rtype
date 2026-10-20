#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "rtype/ReturnCodes.hpp"
#include "game/App.hpp"

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
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return RTYPE_ERROR;
    }
    try {
        server::App app(parsePort(argv[1]));

        return app.run();
    } catch (const std::exception &e) {
        std::cerr << "R-Type server: " << e.what() << std::endl;
        return RTYPE_ERROR;
    }
}
