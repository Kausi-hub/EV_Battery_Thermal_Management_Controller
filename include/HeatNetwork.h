#pragma once

#include <vector>

class HeatNetwork
{
public:
    void calculateThermalCoupling(std::vector<float>& temperatures, float dt);

private:
    float m_conductionFactor{0.02f};
};