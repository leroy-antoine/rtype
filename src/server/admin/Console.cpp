#include <iostream>
#include <thread>
#include "Console.hpp"

namespace server {
    Console::Console()
        : _inbox(std::make_shared<Inbox>())
    {
        std::thread([inbox = _inbox] {
            std::string line;

            while (std::getline(std::cin, line)) {
                const std::lock_guard lock(inbox->mutex);
                inbox->lines.push_back(std::move(line));
            }
        }).detach();
    }

    std::vector<std::string> Console::take()
    {
        std::vector<std::string> lines;
        const std::lock_guard lock(_inbox->mutex);

        lines.swap(_inbox->lines);
        return lines;
    }
}
