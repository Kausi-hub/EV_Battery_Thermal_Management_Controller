#pragma once

#include <array>
#include <string>
#include <cstdint>

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
    float packAverageTemp;

    float maxCellTemp;

    float minCellTemp;

    float cellImbalance;

    float hotspotTemperature;

    std::size_t hotspotModule;

    std::size_t hotspotCell;

    float thermalPropagationIndex;

    FaultType fault;
};