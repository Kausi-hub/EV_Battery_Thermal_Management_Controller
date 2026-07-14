#pragma once

#include "Calibration.h"
#include "IFanDriver.h"
#include "IPumpDriver.h"
#include "ITemperatureSensor.h"
#include "thermal_inputs.h"

enum class ThermalState
{
    STARTUP,
    NORMAL,
    COOLING,
    CRITICAL,
    SAFE
};

struct ThermalCommands
{
    float pumpPercent = 0.0f;
    float fanPercent = 0.0f;
};

class ThermalController
{
public:

    ThermalController(
        ITemperatureSensor& sensor,
        IPumpDriver& pump,
        IFanDriver& fan,
        const ThermalCalibration& calibration);

    ~ThermalController() = default;

    ThermalCommands update(
        const ThermalInputs& inputs,
        float dt);

    void enterSafeMode();

    ThermalState getState() const;

private:

    void updateStateMachine(
        float maxTemp);

    float calculateFeedForward(
        float batteryCurrentA) const;

private:

    ITemperatureSensor& m_sensor;

    IPumpDriver& m_pump;

    IFanDriver& m_fan;

    const ThermalCalibration& m_cal;

    ThermalState m_state;
};