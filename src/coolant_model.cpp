#include "coolant_model.h"

float CoolantModel::update(
    float batteryTemp,
    float pumpPercent,
    float fanPercent,
    float dt)
{
    float heatTransfer = (batteryTemp - m_coolantTemp) * pumpPercent * 0.002f;

    float radiatorCooling = fanPercent * 0.01f;

    m_coolantTemp += (heatTransfer - radiatorCooling) * dt;

    return m_coolantTemp;
}