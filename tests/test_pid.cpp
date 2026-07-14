#include <gtest/gtest.h>

#include "pid_controller.h"

TEST(PidController, OutputIncreasesWithHigherTemperature)
{
    PidController pid(
        2.0f,
        0.1f,
        0.05f,
        0.0f,
        100.0f);

    float lowOutput =
        pid.update(
            35.0f,
            36.0f,
            0.1f);

    float highOutput =
        pid.update(
            35.0f,
            50.0f,
            0.1f);

    EXPECT_GT(
        highOutput,
        lowOutput);
}

TEST(PidController, OutputIsSaturated)
{
    PidController pid(
        10.0f,
        5.0f,
        1.0f,
        0.0f,
        100.0f);

    float output =
        pid.update(
            35.0f,
            200.0f,
            0.1f);

    EXPECT_LE(
        output,
        100.0f);
}

TEST(PidController, OutputNeverNegative)
{
    PidController pid(
        2.0f,
        0.1f,
        0.05f,
        0.0f,
        100.0f);

    float output =
        pid.update(
            35.0f,
            0.0f,
            0.1f);

    EXPECT_GE(
        output,
        0.0f);
}