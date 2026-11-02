#include <algorithm>
#include "logic/Builder.hpp"
#include "logic/MapHelpers.hpp"

namespace rtype {
    namespace {
        using BK = BossKind;

        const std::vector<Gap> WARM_UP = {
            {540.f, 300.f}, {340.f, 280.f}, {640.f, 280.f},
            {300.f, 260.f}, {680.f, 260.f}, {420.f, 240.f}};

        const std::vector<Gap> SLALOM = {
            {540.f, 300.f}, {340.f, 280.f}, {640.f, 280.f},
            {300.f, 260.f}, {680.f, 260.f}, {420.f, 240.f},
            {260.f, 240.f}, {700.f, 220.f}, {380.f, 200.f},
            {620.f, 200.f}, {260.f, 180.f}};

        const std::vector<Gap> NEEDLE = {
            {540.f, 240.f}, {360.f, 240.f}, {620.f, 230.f},
            {280.f, 230.f}, {700.f, 220.f}, {420.f, 220.f},
            {200.f, 210.f}, {560.f, 210.f}, {760.f, 200.f},
            {360.f, 200.f}, {640.f, 190.f}, {240.f, 190.f},
            {520.f, 180.f}, {800.f, 170.f}, {420.f, 160.f},
            {260.f, 160.f}};

        LevelDef run(const float length, const std::vector<Gap> &gaps,
            const GateStyle &style)
        {
            LevelDef level{length, {}, {}, {}};

            addGates(level.obstacles, gaps, style);
            return level;
        }

        std::vector<Gap> reversed(std::vector<Gap> gaps)
        {
            std::ranges::reverse(gaps);
            return gaps;
        }
    }

    std::vector<LevelDef> map3()
    {
        return {
            run(4600.f, WARM_UP, {600.f, 700.f, 96.f, ObstacleKind::Hazard}),
            run(4900.f, SLALOM, {600.f, 380.f, 96.f, ObstacleKind::Hazard}),
            run(5900.f, NEEDLE, {500.f, 320.f, 80.f, ObstacleKind::Hazard}),
            run(5900.f, reversed(NEEDLE),
                {500.f, 320.f, 80.f, ObstacleKind::Hazard}),
            {2000.f, {}, {{BK::Fortress, 1500.f}, {BK::Dasher, 1700.f}}, {}}};
    }
}
