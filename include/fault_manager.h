#pragma once

enum class FaultType
{
    NONE,
    SENSOR_OUT_OF_RANGE,
    OVER_TEMPERATURE
};

class FaultManager
{
public:
    FaultType evaluate(float temp);

private:
};