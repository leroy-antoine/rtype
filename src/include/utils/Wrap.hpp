#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace utils {
    // Cuts a text into lines of `columns` characters at most. A line is cut at
    // a space when there is one, in the middle of a word otherwise.
    inline std::vector<std::string> wrap(const std::string &text,
        const std::size_t columns)
    {
        std::vector<std::string> lines;
        std::string rest = text;

        while (columns > 0 && rest.size() > columns) {
            std::size_t cut = rest.rfind(' ', columns);

            if (cut == std::string::npos || cut == 0) {
                cut = columns;
            }
            lines.push_back(rest.substr(0, cut));
            rest.erase(0, cut);
            if (!rest.empty() && rest.front() == ' ') {
                rest.erase(0, 1);
            }
        }
        lines.push_back(rest);
        return lines;
    }
}
