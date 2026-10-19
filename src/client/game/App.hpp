#pragma once

#include <filesystem>

namespace client {
    class App {
    public:
        explicit App(std::filesystem::path assets);

        int run();

    private:
        std::filesystem::path _assets;
    };
}
