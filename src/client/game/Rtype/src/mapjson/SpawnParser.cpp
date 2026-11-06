#include "Names.hpp"
#include "Parsers.hpp"

namespace rtype::mapjson {
    namespace {
        float randomHeight(const Reader &wave)
        {
            if (wave.text("y") != "random") {
                wave.failKey("y", "expected a number or \"random\"");
            }
            return RANDOM_Y;
        }

        float waveHeight(const Reader &wave)
        {
            if (!wave.has("y")) {
                return RANDOM_Y;
            }
            if (wave.isText("y")) {
                return randomHeight(wave);
            }
            return wave.number("y", WAVE_Y);
        }

        // The i-th monster is at y + i * dy: the last one must stay on screen.
        void checkReach(const Reader &wave, const WaveStep &step)
        {
            const float last = step.y
                + static_cast<float>(step.count - 1) * step.dy;

            if (step.y < 0.f || step.shape != Shape::Line) {
                return;
            }
            if (last < WAVE_Y.min || last > WAVE_Y.max) {
                wave.fail("the last monster would be at y = "
                    + std::to_string(last) + ", outside of the screen");
            }
        }
    }

    WaveStep parseWave(const Reader &wave, const float length)
    {
        WaveStep step{};

        wave.allow({"at", "kind", "count", "y", "dy", "interval", "shape"});
        step.at = wave.number("at", {0.f, length});
        step.kind = wave.choice("kind", MONSTER_NAMES);
        step.count = wave.integer("count", WAVE_COUNT);
        step.y = waveHeight(wave);
        step.dy = wave.numberOr("dy", WAVE_DY, 0.f);
        step.interval = wave.numberOr("interval", WAVE_INTERVAL, 0.5f);
        step.shape = wave.choiceOr("shape", SHAPE_NAMES, Shape::Line);
        checkReach(wave, step);
        return step;
    }

    BossSpawn parseBoss(const Reader &boss, const float length)
    {
        boss.allow({"at", "kind"});
        return {boss.choice("kind", BOSS_NAMES),
            boss.number("at", {0.f, length})};
    }
}
