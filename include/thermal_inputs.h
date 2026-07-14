#pragma once

#include <array>

struct ThermalInputs
{
    std::array<float, 4> zoneTempsC;

    float coolantTempC;

    float batteryCurrentA;

    float pumpRpmFeedback;
    float fanRpmFeedback;
};