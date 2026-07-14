#include <gtest/gtest.h>

#include "diagnostics.h"

TEST(Diagnostics, DetectThermalImbalance)
{
    Diagnostics diagnostics;

    std::array<float,4> zones =
    {
        35.0f,
        35.0f,
        60.0f,
        35.0f
    };

    auto fault =
        diagnostics.evaluate(
            zones,
            30.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.1f);

    EXPECT_EQ(
        fault,
        FaultType::THERMAL_IMBALANCE);
}

TEST(Diagnostics, DetectPumpFailure)
{
    Diagnostics diagnostics;

    std::array<float,4> zones =
    {
        35.0f,
        35.0f,
        35.0f,
        35.0f
    };

    FaultType fault =
        FaultType::NONE;

    for(int i = 0; i < 120; ++i)
    {
        float temp = 35.0f + 0.01f * i;
        std::array<float,4> zones ={temp,temp,temp,temp};
        fault =
            diagnostics.evaluate(
                zones,
                30.0f,
                80.0f,
                0.0f,
                0.0f,
                0.0f,
                0.1f);
    }

    EXPECT_EQ(
        fault,
        FaultType::PUMP_FAILURE);
}

TEST(Diagnostics, DetectFanFailure)
{
    Diagnostics diagnostics;

    std::array<float,4> zones =
    {
        35.0f,
        35.0f,
        35.0f,
        35.0f
    };

    FaultType fault =
        FaultType::NONE;

    for(int i = 0; i < 120; ++i)
    {
        fault =
            diagnostics.evaluate(
                zones,
                30.0f,
                0.0f,
                80.0f,
                0.0f,
                0.0f,
                0.1f);
    }

    EXPECT_EQ(
        fault,
        FaultType::FAN_FAILURE);
}

TEST(Diagnostics, DetectOutOfRangeTemperature)
{
    Diagnostics diagnostics;

    std::array<float,4> zones =
    {
        -100.0f,
        35.0f,
        35.0f,
        35.0f
    };

    auto fault =
        diagnostics.evaluate(
            zones,
            25.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.1f);

    EXPECT_EQ(
        fault,
        FaultType::SENSOR_OUT_OF_RANGE);
}