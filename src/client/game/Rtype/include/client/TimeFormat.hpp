#pragma once

#include <cstdint>
#include <string>

namespace client {
    inline std::string twoDigits(const std::uint32_t value)
    {
        std::string text = std::to_string(value);

        if (text.size() < 2) {
            text.insert(0, "0");
        }
        return text;
    }

    // A time in milliseconds as MM:SS.T
    inline std::string formatTime(const std::uint32_t ms)
    {
        constexpr std::uint32_t MS_PER_MINUTE = 60000;
        constexpr std::uint32_t MS_PER_SECOND = 1000;
        constexpr std::uint32_t MS_PER_TENTH = 100;
        const std::uint32_t minutes = ms / MS_PER_MINUTE;
        const std::uint32_t seconds = ms / MS_PER_SECOND % 60;
        const std::uint32_t tenths = ms / MS_PER_TENTH % 10;

        return twoDigits(minutes) + ":" + twoDigits(seconds) + "."
            + std::to_string(tenths);
    }
}
