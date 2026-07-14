#pragma once

class IPumpDriver
{
public:

    virtual ~IPumpDriver() = default;

    virtual void setSpeed(
        float percent) = 0;

    virtual float getRpm() = 0;
};