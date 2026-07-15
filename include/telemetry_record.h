#pragma once

#include <array>
#include <string>

#include "fault_types.h"

struct TelemetryRecord
{
    uint32_t timestampMs;

    std::array<float, 4> zoneTemps;

    float coolantTemp;

    float batteryCurrent;

    float pumpCommand;
    float fanCommand;

    float pumpRpm;
    float fanRpm;

    FaultType fault;
};