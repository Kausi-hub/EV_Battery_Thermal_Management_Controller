#pragma once

class CoolantModel
{
public:

    float update(
        float avgBatteryTemp,
        float pumpPercent,
        float fanPercent,
        float dt);

private:

    float m_coolantTemp = 25.0f;
};