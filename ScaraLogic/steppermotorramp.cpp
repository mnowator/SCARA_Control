#include "steppermotorramp.h"
#include <cmath>

StepperMotorRamp::StepperMotorRamp()
{

}

double StepperMotorRamp::accumulateTimeForSteps(unsigned steps)
/* Calculation use geometric stange things xd*/
{
    double accumulatedTime = steps/m_targetVelocity;
    double rampUpTime = (m_targetVelocity - m_startVelocity)/m_acceleration;
    double rampDownTime = (m_targetVelocity - m_stopVelocity)/m_deceleration;

    return accumulatedTime;
}

void StepperMotorRamp::setStartVelovity(unsigned velocity)
{
    m_startVelocity = velocity;
}

void StepperMotorRamp::setStopVelocity(unsigned velocity)
{
    m_stopVelocity = velocity;
}

void StepperMotorRamp::setTargetVelocity(unsigned velocity)
{
    m_targetVelocity = velocity;
}

void StepperMotorRamp::setAcceleration(unsigned acceleration)
{
    m_acceleration = acceleration;
}

void StepperMotorRamp::setDeceleration(unsigned deceleration)
{
    m_deceleration = deceleration;
}

void StepperMotorRamp::setRampDownTime(double time)
{
    m_rampDownTime = time;
}

void StepperMotorRamp::setRampUpTime(double time)
{
    m_rampUpTime = time;
}

unsigned StepperMotorRamp::getStartVelocity() const
{
    return m_startVelocity;
}

unsigned StepperMotorRamp::getStopVelovity() const
{
    return m_stopVelocity;
}

unsigned StepperMotorRamp::getMaxiumumVelocity() const
{
    return m_maximumVelocity;
}

unsigned StepperMotorRamp::getAcceleration() const
{
    return m_acceleration;
}

unsigned StepperMotorRamp::getDeceleration() const
{
    return m_deceleration;
}

double StepperMotorRamp::getRampDownTime() const
{
    return m_rampDownTime;
}

double StepperMotorRamp::getRampUpTime() const
{
    return m_rampUpTime;
}

