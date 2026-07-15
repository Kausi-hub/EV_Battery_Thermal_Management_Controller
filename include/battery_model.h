#pragma once

#include <array>
#include <cstddef>

class BatteryModel
{
public:

    static constexpr std::size_t NumZones = 4;

    struct ZoneData
    {
        std::array<float, NumZones> temps;
    };

    ZoneData update(
        const std::array<float, NumZones>& currents,
        float coolantTemp,
        float pumpPercent,
        float dt);

private:

    ZoneData m_zoneData
    {
        {35.0f, 35.0f, 35.0f, 35.0f}
    };
};