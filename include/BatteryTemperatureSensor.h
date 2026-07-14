#pragma once

#include "ITemperatureSensor.h"

class BatteryTemperatureSensor
    : public ITemperatureSensor
{
public:

    float readTemperature() override;
};