#include <gtest/gtest.h>

#include "diagnostics.h"

TEST(Diagnostics, FanFailureDetected)
{
    Diagnostics diagnostics;

    bool fault = false;

    for(int i = 0; i < 120; ++i)
    {
        fault =
            diagnostics.checkFanFailure(
                80.0f,
                0.0f);
    }

    EXPECT_TRUE(fault);
}

TEST(Diagnostics, ThermalImbalanceDetected)
{
    Diagnostics diagnostics;

    std::array<float,4> zones
    {
        35.0f,
        35.0f,
        60.0f,
        35.0f
    };

    EXPECT_TRUE(
        diagnostics.checkThermalImbalance(
            zones));
}

TEST(Diagnostics, BalancedPack)
{
    Diagnostics diagnostics;

    std::array<float,4> zones
    {
        35.0f,
        36.0f,
        35.5f,
        35.2f
    };

    EXPECT_FALSE(
        diagnostics.checkThermalImbalance(
            zones));
}

TEST(Diagnostics, TemperatureOutOfRange)
{
    Diagnostics diagnostics;

    std::array<float,4> zones
    {
        150.0f,
        35.0f,
        35.0f,
        35.0f
    };

    EXPECT_TRUE(
        diagnostics.checkRange(
            zones));
}

TEST(Diagnostics, TemperatureWithinRange)
{
    Diagnostics diagnostics;

    std::array<float,4> zones
    {
        35.0f,
        35.0f,
        35.0f,
        35.0f
    };

    EXPECT_FALSE(
        diagnostics.checkRange(
            zones));
}

TEST(Diagnostics, CoolingIneffective)
{
    Diagnostics diagnostics;

    EXPECT_TRUE(
        diagnostics.checkCoolingIneffective(
            45.0f,
            44.0f,
            3500.0f,
            2500.0f));
}

TEST(Diagnostics, PumpHealthy)
{
    Diagnostics diagnostics;

    bool fault = false;

    for(int i = 0; i < 120; ++i)
    {
        fault = diagnostics.checkPumpFailure(80.0f,3500.0f);
    }

    EXPECT_FALSE(fault);
}

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

TEST(Diagnostics, FanFailureDetected)
{
    Diagnostics diagnostics;

    bool fault = false;

    for(int i = 0; i < 120; ++i)
    {
        fault =
            diagnostics.checkFanFailure(
                80.0f,
                0.0f);
    }

    EXPECT_TRUE(fault);
}