#include "thermal_controller.h"
#include "pid_controller.h"
#include "ITemperatureSensor.h"
#include "IPumpDriver.h"
#include "IFanDriver.h"
#include <algorithm>

namespace
{
PidController g_pid(
    2.0f,
    0.1f,
    0.05f,
    0.0f,
    100.0f);

struct PIDGains
{
    float kp;
    float ki;
    float kd;
};

PIDGains selectGains(float temperature)
{
    if (temperature < 40.0f)
    {
        return {2.0f, 0.1f, 0.05f};
    }

    if (temperature < 50.0f)
    {
        return {5.0f, 0.3f, 0.1f};
    }

    return {8.0f, 0.5f, 0.2f};
}
}

ThermalController::ThermalController(
    ITemperatureSensor& sensor,
    IPumpDriver& pump,
    IFanDriver& fan,
    const ThermalCalibration& calibration): 
    m_sensor(sensor), 
    m_pump(pump),
    m_fan(fan),
    m_cal(calibration),
    m_state(ThermalState::STARTUP)
{
}

ThermalState ThermalController::getState() const
{
    return m_state;
}

void ThermalController::enterSafeMode()
{
    m_state = ThermalState::SAFE;

    m_pump.setSpeed(100.0f);
    m_fan.setSpeed(100.0f);
}

float ThermalController::calculateFeedForward(float batteryCurrentA) const
{
    constexpr float batteryResistance = 0.02f;
    constexpr float gain = 0.0001f;

    float powerLoss =
        batteryCurrentA *
        batteryCurrentA *
        batteryResistance;

    return std::clamp(
        powerLoss * gain,
        0.0f,
        80.0f);
}

void ThermalController::updateStateMachine(float maxTemp)
{
    switch (m_state)
    {
    case ThermalState::STARTUP:

        m_state =
            (maxTemp < m_cal.normalThreshold)
            ? ThermalState::NORMAL
            : ThermalState::COOLING;
        break;

    case ThermalState::NORMAL:

        if (maxTemp >= m_cal.normalThreshold)
        {
            m_state = ThermalState::COOLING;
        }
        break;

    case ThermalState::COOLING:

        if (maxTemp >= m_cal.criticalThreshold)
        {
            m_state = ThermalState::CRITICAL;
        }
        else if (maxTemp < (m_cal.normalThreshold - 2.0f))
        {
            m_state = ThermalState::NORMAL;
        }
        break;

    case ThermalState::CRITICAL:

        if (maxTemp >= m_cal.safeThreshold)
        {
            m_state = ThermalState::SAFE;
        }
        break;

    case ThermalState::SAFE:
        break;
    }
}

ThermalCommands ThermalController::update(const ThermalInputs& inputs, float dt)
{
    float maxTemp =
        *std::max_element(
            inputs.zoneTempsC.begin(),
            inputs.zoneTempsC.end());

    updateStateMachine(maxTemp);

    ThermalCommands cmd{};

    if (m_state == ThermalState::SAFE)
    {
        cmd.pumpPercent = 100.0f;
        cmd.fanPercent = 100.0f;

        m_pump.setSpeed(cmd.pumpPercent);
        m_fan.setSpeed(cmd.fanPercent);

        return cmd;
    }

    auto gains = selectGains(maxTemp);

    g_pid.setGains(
        gains.kp,
        gains.ki,
        gains.kd);

    float pidDemand =
        g_pid.update(
            m_cal.targetTemp,
            maxTemp,
            dt);

    float ffDemand =
        calculateFeedForward(
            inputs.batteryCurrentA);

    float demand = std::clamp(
        pidDemand + ffDemand,
        0.0f,
        100.0f);

    switch (m_state)
    {
    case ThermalState::NORMAL:
        cmd.pumpPercent = 15.0f;
        cmd.fanPercent = 0.0f;
        break;

    case ThermalState::COOLING:
        cmd.pumpPercent = demand;
        cmd.fanPercent = demand;
        break;

    case ThermalState::CRITICAL:
        cmd.pumpPercent = 100.0f;
        cmd.fanPercent = 100.0f;
        break;

    default:
        break;
    }

    m_pump.setSpeed(cmd.pumpPercent);
    m_fan.setSpeed(cmd.fanPercent);

    return cmd;
}