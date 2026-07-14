#include <gtest/gtest.h>

#include "thermal_controller.h"
#include "MockPumpDriver.h"
#include "MockTemperatureSensor.h"
#include "IFanDriver.h"

class MockFanDriver : public IFanDriver
{
public:

    void setSpeed(float percent) override
    {
        lastCommand = percent;
    }

    float getRpm() override
    {
        return lastCommand * 30.0f;
    }

    float lastCommand = 0.0f;
};

class DummyTemperatureSensor
    : public ITemperatureSensor
{
public:

    float readTemperature() override
    {
        return 45.0f;
    }
};

TEST(ThermalController, CoolingCommandProduced)
{
    DummyTemperatureSensor sensor;

    MockPumpDriver pump;
    MockFanDriver fan;

    ThermalCalibration cal
    {
        35.0f,
        40.0f,
        50.0f,
        55.0f
    };

    ThermalController controller(
        sensor,
        pump,
        fan,
        cal);

    ThermalInputs inputs;

    inputs.zoneTempsC =
    {
        45.0f,
        44.0f,
        43.0f,
        42.0f
    };

    inputs.coolantTempC = 25.0f;
    inputs.batteryCurrentA = 250.0f;
    inputs.pumpRpmFeedback = 0.0f;
    inputs.fanRpmFeedback = 0.0f;

    auto cmd =
        controller.update(
            inputs,
            0.1f);

    EXPECT_GT(
        cmd.pumpPercent,
        0.0f);

    EXPECT_GT(
        cmd.fanPercent,
        0.0f);
}

TEST(ThermalController, SafeModeCommandsFullCooling)
{
    DummyTemperatureSensor sensor;

    MockPumpDriver pump;
    MockFanDriver fan;

    ThermalCalibration cal
    {
        35.0f,
        40.0f,
        50.0f,
        55.0f
    };

    ThermalController controller(
        sensor,
        pump,
        fan,
        cal);

    controller.enterSafeMode();

    ThermalInputs inputs;

    inputs.zoneTempsC =
    {
        60.0f,
        60.0f,
        60.0f,
        60.0f
    };

    auto command =
        controller.update(
            inputs,
            0.1f);

    EXPECT_FLOAT_EQ(
        command.pumpPercent,
        100.0f);

    EXPECT_FLOAT_EQ(
        command.fanPercent,
        100.0f);
}