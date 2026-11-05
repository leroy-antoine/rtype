#pragma once

#include <algorithm>
#include <cstddef>
#include "Config.hpp"
#include "Levels.hpp"

namespace rtype {
    struct SkipBank {
        std::size_t level = 0;
        float skipped = 0.f;
    };

    // Distance gagnee par les kills, plafonnee a une part de chaque niveau.
    inline float grantSkip(const Segments &segments, SkipBank &bank,
        const float scroll, const int kills)
    {
        const std::size_t level = levelAt(segments, scroll);
        const float cap = config::KILL_SKIP_MAX_FRACTION
            * levelSpan(segments, scroll);
        const float wanted = static_cast<float>(kills)
            * config::KILL_SKIP_SECONDS * config::SCROLL_SPEED;

        if (level != bank.level) {
            bank = SkipBank{level, 0.f};
        }
        const float granted = std::clamp(wanted, 0.f, cap - bank.skipped);

        bank.skipped += std::max(granted, 0.f);
        return std::max(granted, 0.f);
    }
}
