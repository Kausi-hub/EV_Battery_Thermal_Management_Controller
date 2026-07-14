#pragma once

#include "IPumpDriver.h"

class MockPumpDriver
    : public IPumpDriver
{
public:

    void setSpeed(
        float percent)
        override
    {
        lastCommand = percent;
    }

    float getRpm()
        override
    {
        return lastCommand * 40.0f;
    }

    float lastCommand = 0.0f;
};