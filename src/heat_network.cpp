#include "HeatNetwork.h"

#include <vector>

void HeatNetwork::calculateThermalCoupling(std::vector<float>& temperatures, float dt)
{
    if (temperatures.size() < 2)
    {
        return;
    }

    std::vector<float> updatedTemps = temperatures;

    for (std::size_t i = 0; i < temperatures.size(); ++i)
    {
        float heatFlow = 0.0f;

        if (i > 0)
        {
            heatFlow += m_conductionFactor * (temperatures[i - 1] - temperatures[i]);
        }

        if (i < temperatures.size() - 1)
        {
            heatFlow += m_conductionFactor * (temperatures[i + 1] - temperatures[i]);
        }

        updatedTemps[i] += heatFlow * dt;
    }

    temperatures = updatedTemps;
}