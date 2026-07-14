#include "fault_manager.h"

FaultType FaultManager::evaluate(
    float temp)
{
    if (temp < -40.0f ||
        temp > 120.0f)
    {
        return FaultType::SENSOR_OUT_OF_RANGE;
    }

    if (temp > 55.0f)
    {
        return FaultType::OVER_TEMPERATURE;
    }

    return FaultType::NONE;
}