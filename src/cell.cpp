#include "Cell.h"

Cell::Cell(float capacityAh, float internalResistance)
    : m_capacityAh(capacityAh),
      m_internalResistance(internalResistance)
{
}

void Cell::update(float current, float ambientTemp, float coolingFactor, float dt)
{
    // Joule heating
    m_heatGeneration = current * current * m_internalResistance;

    // Simple convective cooling
    const float cooling = (m_temperature - ambientTemp) * coolingFactor;

    // Lumped thermal model
    m_temperature += (m_heatGeneration - cooling) * dt;

    // Very simple voltage sag model
    m_voltage = 3.7f - (current * m_internalResistance);
}

float Cell::getTemperature() const
{
    return m_temperature;
}

float Cell::getVoltage() const
{
    return m_voltage;
}

float Cell::getHeatGeneration() const
{
    return m_heatGeneration;
}

bool Cell::isHotSpot(float threshold) const
{
    return m_temperature >= threshold;
}

void Cell::setTemperature(float temp)
{
    m_temperature = temp;
}