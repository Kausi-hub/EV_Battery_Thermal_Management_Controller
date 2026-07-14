#include "PumpDriver.h"

void PumpDriver::setSpeed(float percent)
{
    m_speedPercent = percent;
}

float PumpDriver::getRpm()
{
    return m_speedPercent * 40.0f;
}