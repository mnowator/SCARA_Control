#include "steppermotorramp.h"
#include <cmath>

#include <QDebug>

StepperMotorRamp::StepperMotorRamp(QObject *parent)
    : QObject(parent)
{

}

StepperMotorRamp &StepperMotorRamp::operator ++()
{
    ++m_accumulatedSteps;
}

StepperMotorRamp &StepperMotorRamp::operator --()
{
    if ( m_accumulatedSteps > 0 )
        --m_accumulatedSteps;
}

double StepperMotorRamp::accumulatedTime()
{
    qDebug() << m_accumulatedSteps << "steps";

    m_accumulatedTime = abs( m_accumulatedSteps/m_targetVelocity );

    return m_accumulatedTime;
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

void StepperMotorRamp::setAcumulatedSteps(int steps)
{
    m_accumulatedSteps = steps;
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
    return m_targetVelocity;
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

