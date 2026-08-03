#pragma once

#include <vector>
#include "Module.h"
#include "HeatNetwork.h"

class BatteryPack
{
public:
    explicit BatteryPack(
        std::size_t moduleCount,
        std::size_t cellsPerModule);

    void update(
        float packCurrent,
        float coolantTemp,
        float pumpPercent,
        float dt);

    float getPackAverageTemp() const;

    float getMaxTemperature() const;

    float getCellImbalance() const;

    bool detectThermalPropagation() const;
    struct Hotspot
    {
    std::size_t moduleIdx{0};
    std::size_t cellIdx{0};
    float temperature{0.0f};
    };

private:
    std::vector<Module> m_modules;
    HeatNetwork m_heatNetwork;
    HeatNetwork m_heatNetwork;
    Hotspot m_hotspot;

    float m_propagationIndex{0.0f};
};
