#pragma once

#include <algorithm>
#include "engine/window/Types.hpp"

namespace Engine {
    struct Animation {
        IntRect firstFrame;
        int frameCount = 1;
        int step = 0;
        float frameDuration = 0.1f;

        [[nodiscard]] IntRect frame(const int index) const
        {
            IntRect rect = firstFrame;

            rect.left += std::clamp(index, 0, frameCount - 1) * (step > 0 ? step : firstFrame.width);
            return rect;
        }

        [[nodiscard]] IntRect frameAt(const float elapsed) const
        {
            if (frameCount <= 1 || frameDuration <= 0.f)
                return firstFrame;
            return frame(static_cast<int>(elapsed / frameDuration) % frameCount);
        }

        [[nodiscard]] IntRect frameAtProgress(const float progress) const
        {
            return frame(static_cast<int>(progress * static_cast<float>(frameCount)));
        }
    };
}
