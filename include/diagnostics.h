#pragma once

#include <array>

#include "fault_types.h"

class Diagnostics
{
public:

    Diagnostics();

    bool checkRange(
        const std::array<float,4>& zoneTemps);

    bool checkPlausibility(
        const std::array<float,4>& zoneTemps,
        float coolantTemp,
        float dt);

    bool checkPumpFailure(
        float pumpCommand,
        float pumpRpm);

    bool checkFanFailure(
        float fanCommand,
        float fanRpm);

    bool checkCoolingIneffective(
        float currentTemp,
        float previousTemp,
        float pumpRpm,
        float fanRpm);

    bool checkThermalImbalance(
        const std::array<float,4>& zoneTemps);

    FaultType evaluate(
        const std::array<float,4>& zoneTemps,
        float coolantTemp,
        float pumpCommand,
        float fanCommand,
        float pumpRpm,
        float fanRpm,
        float dt);

private:

    bool m_firstCycle;

    float m_previousMaxTemp;

    int m_stuckCounter;
    int m_pumpFaultCounter;
    int m_fanFaultCounter;
    int m_coolingCounter;
};