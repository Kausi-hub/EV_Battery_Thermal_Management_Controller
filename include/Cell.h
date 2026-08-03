#pragma once

class Cell
{
public:
    Cell(float capacityAh,
         float internalResistance);

    void update(
        float current,
        float ambientTemp,
        float coolingFactor,
        float dt);

    float getTemperature() const;
    float getVoltage() const;
    float getHeatGeneration() const;
    void setTemperature(float temp);


    bool isHotSpot(float threshold) const;

private:
    float m_temperature{25.0f};
    float m_voltage{3.7f};

    float m_capacityAh;
    float m_internalResistance;

    float m_heatGeneration{0.0f};
};