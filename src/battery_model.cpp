#include "battery_model.h"
#include <cstddef>

BatteryModel::ZoneData BatteryModel::update(
    const std::array<float, BatteryModel::NumZones>& currents,
    float coolantTemp,
    float pumpPercent,
    float dt)
{
    constexpr float resistance = 0.02f;
    static constexpr std::size_t NumZones = 4;

    for(std::size_t i = 0; i < BatteryModel::NumZones; i++)
    {
        float heatGen = currents[i] * currents[i] * resistance * 0.0002f;
        float cooling = (m_zoneData.temps[i] - coolantTemp) * pumpPercent * 0.0005f;
        m_zoneData.temps[i] += (heatGen - cooling) * dt;
    }

    return m_zoneData;
}