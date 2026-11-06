#pragma once

#include <algorithm>
#include <string>

namespace rtype::mapjson {
    // True when the text holds no control character. Shared by the reader of
    // the maps and by the network decoders, so both refuse the same texts.
    inline bool isPrintableText(const std::string &text)
    {
        return std::ranges::all_of(text, [](const char c) {
            const auto code = static_cast<unsigned char>(c);

            return code >= 0x20 && code != 0x7F;
        });
    }
}
