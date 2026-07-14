#pragma once

#include "ITemperatureSensor.h"

class MockTemperatureSensor
    : public ITemperatureSensor
{
public:

    explicit MockTemperatureSensor(
        float temp)
        :
        m_temp(temp)
    {
    }

    float readTemperature()
        override
    {
        return m_temp;
    }

private:

    float m_temp;
};