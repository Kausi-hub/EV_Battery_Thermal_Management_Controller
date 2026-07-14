#pragma once

class IFanDriver
{
public:

    virtual ~IFanDriver() = default;

    virtual void setSpeed(
        float percent) = 0;

    virtual float getRpm() = 0;
};