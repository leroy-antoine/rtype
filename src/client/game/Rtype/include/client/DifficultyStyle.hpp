#pragma once

#include <cmath>
#include <cstddef>
#include <string>
#include "engine/window/Types.hpp"
#include "logic/Settings.hpp"

namespace client {
    inline std::string difficultyName(const rtype::Difficulty difficulty)
    {
        return rtype::DIFFICULTY_NAMES[static_cast<std::size_t>(difficulty)];
    }

    // Du vert au carmin : plus la difficulte monte, plus la couleur chauffe.
    inline Engine::Color difficultyColor(const rtype::Difficulty difficulty)
    {
        switch (difficulty) {
            case rtype::Difficulty::Easy: return {90, 220, 90, 255};
            case rtype::Difficulty::Normal: return {255, 170, 40, 255};
            case rtype::Difficulty::Hard: return {235, 70, 60, 255};
            case rtype::Difficulty::Brutal: return {200, 16, 52, 255};
        }
        return {255, 255, 255, 255};
    }

    // Un multiplicateur affiche avec au plus deux decimales : X1.5, X0.75.
    inline std::string multiplierText(const float factor)
    {
        const int hundredths = static_cast<int>(std::lround(factor * 100.f));
        std::string decimals = std::to_string(hundredths % 100 + 100);

        decimals = decimals.substr(1);
        if (decimals.back() == '0') {
            decimals.pop_back();
        }
        if (decimals.empty() || decimals == "0") {
            return "X" + std::to_string(hundredths / 100);
        }
        return "X" + std::to_string(hundredths / 100) + "." + decimals;
    }
}
