#include "diagnostics.h"

#include <algorithm>
#include <cmath>

namespace
{
constexpr float MIN_VALID_TEMP = -40.0f;
constexpr float MAX_VALID_TEMP = 120.0f;
constexpr float MAX_SENSOR_DELTA = 60.0f;
constexpr float MAX_TEMP_RATE = 5.0f;      // C/sec
constexpr float MAX_ZONE_DELTA = 20.0f;
constexpr float STUCK_EPSILON = 0.05f;
constexpr int STUCK_LIMIT = 50;
constexpr int FEEDBACK_LIMIT = 100;
constexpr int COOLING_LIMIT = 300;
}

Diagnostics::Diagnostics():
m_firstCycle(true),
m_previousMaxTemp(0.0f),
m_stuckCounter(0),
m_pumpFaultCounter(0),
m_fanFaultCounter(0),
m_coolingCounter(0)
{
}

bool Diagnostics::checkRange(
    const std::array<float,4>& zoneTemps)
{
    for(float temp : zoneTemps)
    {
        if(temp < MIN_VALID_TEMP)
        {
            return true;
        }

        if(temp > MAX_VALID_TEMP)
        {
            return true;
        }
    }

    return false;
}

bool Diagnostics::checkPlausibility(
    const std::array<float,4>& zoneTemps,
    float coolantTemp,
    float dt)
{
    float maxTemp =
        *std::max_element(
            zoneTemps.begin(),
            zoneTemps.end());

    if(std::fabs(maxTemp - coolantTemp)
       > MAX_SENSOR_DELTA)
    {
        return true;
    }

    if(m_firstCycle)
    {
        return false;
    }

    float rate =
        (maxTemp - m_previousMaxTemp)
        / dt;

    if(std::fabs(rate)
       > MAX_TEMP_RATE)
    {
        return true;
    }

    if(std::fabs(
       maxTemp - m_previousMaxTemp)
       < STUCK_EPSILON)
    {
        m_stuckCounter++;
    }
    else
    {
        m_stuckCounter = 0;
    }

    if(m_stuckCounter > STUCK_LIMIT)
    {
        return true;
    }

    return false;
}

bool Diagnostics::checkThermalImbalance(
    const std::array<float,4>& zoneTemps)
{
    float minTemp =
        *std::min_element(
            zoneTemps.begin(),
            zoneTemps.end());

    float maxTemp =
        *std::max_element(
            zoneTemps.begin(),
            zoneTemps.end());

    return
        (maxTemp - minTemp)
        > MAX_ZONE_DELTA;
}

bool Diagnostics::checkPumpFailure(
    float pumpCommand,
    float pumpRpm)
{
    bool faultCondition =
        (pumpCommand > 50.0f)
        &&
        (pumpRpm < 1000.0f);

    if(faultCondition)
    {
        m_pumpFaultCounter++;
    }
    else
    {
        m_pumpFaultCounter = 0;
    }

    return
        m_pumpFaultCounter
        > FEEDBACK_LIMIT;
}

bool Diagnostics::checkFanFailure(
    float fanCommand,
    float fanRpm)
{
    bool faultCondition =
        (fanCommand > 50.0f)
        &&
        (fanRpm < 500.0f);

    if(faultCondition)
    {
        m_fanFaultCounter++;
    }
    else
    {
        m_fanFaultCounter = 0;
    }

    return
        m_fanFaultCounter
        > FEEDBACK_LIMIT;
}

bool Diagnostics::checkCoolingIneffective(
    float maxTemp,
    float pumpRpm,
    float fanRpm)
{
    if(m_firstCycle)
    {
        return false;
    }

    bool coolingHealthy =
        pumpRpm > 3000.0f
        &&
        fanRpm > 2000.0f;

    bool tempRising =
        maxTemp >
        m_previousMaxTemp;

    if(coolingHealthy &&
       tempRising)
    {
        m_coolingCounter++;
    }
    else
    {
        m_coolingCounter = 0;
    }

    return
        m_coolingCounter
        > COOLING_LIMIT;
}

FaultType Diagnostics::evaluate(
    const std::array<float,4>& zoneTemps,
    float coolantTemp,
    float pumpCommand,
    float fanCommand,
    float pumpRpm,
    float fanRpm,
    float dt)
{
    float maxTemp =
        *std::max_element(
            zoneTemps.begin(),
            zoneTemps.end());

    if(checkRange(zoneTemps))
    {
        return FaultType::SENSOR_OUT_OF_RANGE;
    }

    if(checkPlausibility(
        zoneTemps,
        coolantTemp,
        dt))
    {
        return FaultType::SENSOR_IMPLAUSIBLE;
    }

    if(checkThermalImbalance(
        zoneTemps))
    {
        return FaultType::THERMAL_IMBALANCE;
    }

    if(checkPumpFailure(
        pumpCommand,
        pumpRpm))
    {
        return FaultType::PUMP_FAILURE;
    }

    if(checkFanFailure(
        fanCommand,
        fanRpm))
    {
        return FaultType::FAN_FAILURE;
    }

    if(checkCoolingIneffective(
        maxTemp,
        pumpRpm,
        fanRpm))
    {
        return FaultType::COOLING_INEFFECTIVE;
    }

    if(maxTemp > 55.0f)
    {
        return FaultType::OVER_TEMPERATURE;
    }

    m_previousMaxTemp = maxTemp;
    m_firstCycle = false;

    return FaultType::NONE;
}