#include "BatteryPack.h"

#include <algorithm>
#include <vector>

BatteryPack::BatteryPack(std::size_t moduleCount, std::size_t cellsPerModule)
{
    m_modules.reserve(moduleCount);

    for (std::size_t i = 0; i < moduleCount; ++i)
    {
        m_modules.emplace_back(cellsPerModule);
    }
}

void BatteryPack::update(float packCurrent, float coolantTemp, float pumpPercent, float dt)
{
    if(m_modules.empty())
    {
        return;
    }

    //-----------------------------------
    // Update electrical / thermal model
    //-----------------------------------

    const float moduleCurrent =
        packCurrent / static_cast<float>(m_modules.size());

    for(auto& module : m_modules)
    {
        module.update(moduleCurrent, coolantTemp, pumpPercent, dt);
    }

    //-----------------------------------
    // Cell-to-cell thermal conduction
    //-----------------------------------

    for(auto& module : m_modules)
    {
        auto cellTemps = module.getCellTemperatures();

        m_heatNetwork.calculateThermalCoupling(cellTemps, dt);

        module.applyCellTemperatures(cellTemps);
    }

    //-----------------------------------
    // Module-to-module thermal conduction
    //-----------------------------------

    std::vector<float> moduleTemps;

    moduleTemps.reserve(m_modules.size());

    for(const auto& module : m_modules)
    {
        moduleTemps.push_back(module.getAverageTemperature());
    }

    m_heatNetwork.calculateThermalCoupling(moduleTemps, dt);

    //-----------------------------------
    // Apply module temperature influence
    //-----------------------------------

    for(std::size_t i = 0; i < m_modules.size(); ++i)
    {
        float delta = moduleTemps[i] - m_modules[i].getAverageTemperature();

        auto cellTemps = m_modules[i].getCellTemperatures();

        for(auto& t : cellTemps)
        {
            t += delta;
        }

        m_modules[i].applyCellTemperatures(cellTemps);
    }

    //-----------------------------------
    // Hotspot tracking
    //-----------------------------------

    float maxTemp = -1000.0f;

    for(std::size_t m = 0; m < m_modules.size(); ++m)
    {
        const auto& cells = m_modules[m].getCells();

        for(std::size_t c = 0; c < cells.size(); ++c)
        {
            float temp = cells[c].getTemperature();

            if(temp > maxTemp)
            {
                maxTemp = temp;

                m_hotspot.moduleIdx = m;
                m_hotspot.cellIdx = c;
                m_hotspot.temperature = temp;
            }
        }
    }

    //-----------------------------------
    // Thermal Runaway Propagation Index
    //-----------------------------------

    float propagation = 0.0f;

    for(const auto& module : m_modules)
    {
        propagation += module.getCellImbalance();
    }

    m_propagationIndex = propagation / static_cast<float>(m_modules.size());
}

float BatteryPack::getPackAverageTemp() const
{
    if (m_modules.empty())
    {
        return 0.0f;
    }

    float total = 0.0f;

    for (const auto& module : m_modules)
    {
        total += module.getAverageTemperature();
    }

    return total / static_cast<float>(m_modules.size());
}

float BatteryPack::getMaxTemperature() const
{
    float maxTemp = -1000.0f;

    for (const auto& module : m_modules)
    {
        maxTemp = std::max(maxTemp, module.getMaxCellTemp());
    }

    return maxTemp;
}

float BatteryPack::getCellImbalance() const
{
    float packImbalance = 0.0f;

    for (const auto& module : m_modules)
    {
        packImbalance = std::max(packImbalance, module.getCellImbalance());
    }

    return packImbalance;
}

bool BatteryPack::detectThermalPropagation() const
{
    constexpr float propagationThreshold = 10.0f;

    for (const auto& module : m_modules)
    {
        if (module.getCellImbalance() > propagationThreshold)
        {
            return true;
        }
    }

    return false;
}