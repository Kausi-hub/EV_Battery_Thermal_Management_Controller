#include <array>
#include <cstdint>
#include <iostream>

#include "BatteryTemperatureSensor.h"
#include "ConfigLoader.h"
#include "Calibration.h"
#include "PumpDriver.h"
#include "IFanDriver.h"
#include "coolant_model.h"
#include "diagnostics.h"
#include "fault_types.h"
#include "thermal_controller.h"
#include "watchdog.h"

// Temporary fan implementation
class FanDriver : public IFanDriver
{
public:
    void setSpeed(float percent) override
    {
        m_percent = percent;
    }

    float getRpm() override
    {
        return m_percent * 30.0f;
    }

private:
    float m_percent = 0.0f;
};

int main()
{
    ThermalCalibration calibration =
        ConfigLoader::load(
            "thermal_calibration.txt");

    BatteryTemperatureSensor sensor;
    PumpDriver pump;
    FanDriver fan;

    ThermalController controller(
        sensor,
        pump,
        fan,
        calibration);

    Diagnostics diagnostics;
    CoolantModel coolant;
    Watchdog watchdog;

    float batteryTemp = 35.0f;
    float coolantTemp = 25.0f;

    uint32_t timeMs = 0;

    for (int cycle = 0;
         cycle < 600;
         ++cycle)
    {
        watchdog.kick(timeMs);

        float current =
            (cycle > 200)
                ? 320.0f
                : 100.0f;

        ThermalInputs inputs;

        inputs.zoneTempsC =
        {
            batteryTemp,
            batteryTemp + 0.5f,
            batteryTemp - 0.4f,
            batteryTemp + 0.3f
        };

        inputs.coolantTempC = coolantTemp;
        inputs.batteryCurrentA = current;

        inputs.pumpRpmFeedback =
            pump.getRpm();

        inputs.fanRpmFeedback =
            fan.getRpm();

        auto commands =
            controller.update(
                inputs,
                0.1f);

        float averageBatteryTemp =
            (inputs.zoneTempsC[0] +
             inputs.zoneTempsC[1] +
             inputs.zoneTempsC[2] +
             inputs.zoneTempsC[3]) / 4.0f;

        coolantTemp =
            coolant.update(
                averageBatteryTemp,
                commands.pumpPercent,
                commands.fanPercent,
                0.1f);

        float heatGeneration =
            current *
            current *
            0.02f *
            0.0002f;

        float heatRemoval =
            (batteryTemp - coolantTemp) *
            commands.pumpPercent *
            0.0005f;

        batteryTemp +=
            (heatGeneration - heatRemoval);

        auto fault =
            diagnostics.evaluate(
                inputs.zoneTempsC,
                coolantTemp,
                commands.pumpPercent,
                commands.fanPercent,
                pump.getRpm(),
                fan.getRpm(),
                0.1f);

        if (fault != FaultType::NONE)
        {
            controller.enterSafeMode();

            std::cout
                << "FAULT DETECTED: "
                << static_cast<int>(fault)
                << std::endl;
        }

        if (watchdog.expired(timeMs))
        {
            controller.enterSafeMode();
        }

        std::cout
            << cycle
            << ","
            << batteryTemp
            << ","
            << coolantTemp
            << ","
            << commands.pumpPercent
            << ","
            << commands.fanPercent
            << std::endl;

        timeMs += 100;
    }

    return 0;
}