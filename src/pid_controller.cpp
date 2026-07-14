#include "pid_controller.h"

#include <algorithm>

PidController::PidController(
    float kp,
    float ki,
    float kd,
    float minOutput,
    float maxOutput)
:
m_kp(kp),
m_ki(ki),
m_kd(kd),
m_integral(0.0f),
m_prevError(0.0f),
m_minOutput(minOutput),
m_maxOutput(maxOutput),
m_integralLimit(500.0f)
{
}

void PidController::setGains(
    float kp,
    float ki,
    float kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

float PidController::update(
    float setpoint,
    float measurement,
    float dt)
{
    float error =
        measurement - setpoint;

    m_integral += error * dt;

    m_integral = std::clamp(
        m_integral,
        -m_integralLimit,
        m_integralLimit);

    float derivative =
        (error - m_prevError) / dt;

    float output =
        m_kp * error +
        m_ki * m_integral +
        m_kd * derivative;

    float saturated =
        std::clamp(
            output,
            m_minOutput,
            m_maxOutput);

    if(output != saturated)
    {
        m_integral -= error * dt;
    }

    m_prevError = error;

    return saturated;
}

/*No gain scheduling*/
/* PidController::PidController(float kp, float ki, float kd, float minOutput, float maxOutput):
    m_kp(kp),
    m_ki(ki),
    m_kd(kd),
    m_integral(0.0f),
    m_prevError(0.0f),
    m_minOutput(minOutput),
    m_maxOutput(maxOutput)
{
}

float PidController::update(float setpoint, float measurement, float dt)
{
    float error = measurement - setpoint;
    m_integral += error * dt;
    float derivative = (error - m_prevError) / dt;
    float output = (m_kp * error) + (m_ki * m_integral) + (m_kd * derivative);
    m_prevError = error;
    return std::clamp( output, m_minOutput, m_maxOutput);
}

void PidController::reset()
{
    m_integral = 0.0f;
    m_prevError = 0.0f;
} */