#pragma once

class PidController
{
public:

    PidController(
        float kp,
        float ki,
        float kd,
        float minOutput,
        float maxOutput);

    float update(
        float setpoint,
        float measurement,
        float dt);

    void setGains(
        float kp,
        float ki,
        float kd);

private:

    float m_kp;
    float m_ki;
    float m_kd;

    float m_integral;
    float m_prevError;

    float m_minOutput;
    float m_maxOutput;

    float m_integralLimit;
};