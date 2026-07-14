#pragma once

#include "IPumpDriver.h"

class PumpDriver : public IPumpDriver
{
public:

    void setSpeed(float percent) override;

    float getRpm() override;

private:

    float m_speedPercent = 0.0f;
};