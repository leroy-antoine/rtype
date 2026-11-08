#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace server {
    class Console {
    public:
        Console();

        std::vector<std::string> take();

    private:
        struct Inbox {
            std::mutex mutex;
            std::vector<std::string> lines;
        };

        std::shared_ptr<Inbox> _inbox;
    };
}
