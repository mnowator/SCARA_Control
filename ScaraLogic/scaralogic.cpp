#include "scaralogic.h"
#include <cmath>
#include <QObject>

ScaraLogic::ScaraLogic()
{
}

void ScaraLogic::computePositionBySteps(unsigned firstMotorSteps, unsigned secondMotorSteps, unsigned thirdMotorSteps)
{

}

//void ScaraLogic::computeAnglePerStepMotor1()
//{
//    m_motor1anglePerStep = (180+m_firstSegmentCWLimitAngle+m_firstSegmentCCWLimitAngle)/m_motor1maxSteps;
//}

//void ScaraLogic::computeAnglePerStepMotor2()
//{
//    m_motor2anglePerStep = (180+m_secondSegmentCWLimitAngle+m_secondSegmentCCWLimitAngle)/m_motor2maxSteps;
//}

//void ScaraLogic::computeDistancePerStepMotor3()
//{
//    m_motor3distPerStep = (m_thirdSegmentLength-m_distanceBetweenTwoLimitsOnZ)/m_motor3maxSteps;
//}

void ScaraLogic::setFirstSegmentLength(double length)
{
    if ( length < MIN_FIRST_SEGMENT_LENGTH )
        throw Exception(QObject::tr("First segment length is less than min value."));

    m_firstSegmentLength = length;
}

void ScaraLogic::setSecondSegmentLength(double length)
{
    if ( length < MIN_SECOND_SEGMENT_LENGTH )
        throw Exception(QObject::tr("Second segment length is less than min value."));

    m_secondSegmentLength = length;
}

void ScaraLogic::setThirdSegmentLength(double length)
{
    if ( length < MIN_THIRD_SEGMENT_LENGTH )
        throw Exception(QObject::tr("Third segment length is less than min value."));

    m_thirdSegmentLength = length;
}

void ScaraLogic::setCorrectionValue(double length)
{
    m_correctionValue = length;
}

void ScaraLogic::setDistanceBetweenTwoLimitsOnZ(double length)
{
    m_distanceBetweenTwoLimitsOnZ = length;
}

void ScaraLogic::setFirstSegmentCWLimitAngle(double angle)
{
    m_firstSegmentCWLimitAngle = std::fmod(angle, 360.0);
}

void ScaraLogic::setFirstSegmentCCWLimitAngle(double angle)
{
    m_firstSegmentCCWLimitAngle = std::fmod(angle, 360.0);
}

void ScaraLogic::setSecondSegmentCWLimitAngle(double angle)
{
    m_secondSegmentCWLimitAngle = std::fmod(angle, 360.0);;
}

void ScaraLogic::setSecondSegmentCCWLimitAngle(double angle)
{
    m_secondSegmentCCWLimitAngle = std::fmod(angle, 360.0);
}

void ScaraLogic::setMotor1maxSteps(unsigned steps)
{
    m_motor1maxSteps = steps;
}

void ScaraLogic::setMotor2maxSteps(unsigned steps)
{
    m_motor2maxSteps = steps;
}

void ScaraLogic::setMotor3maxSteps(unsigned steps)
{
    m_motor3maxSteps = steps;

}

void ScaraLogic::setMotor1HomingSwitchOrientation(ScaraLogic::SWITCH_ORIENTATION so)
{
    m_motor1SwitchOrientation = so;
}

void ScaraLogic::setMotor2HomingSwitchOrientation(ScaraLogic::SWITCH_ORIENTATION so)
{
    m_motor2SwitchOrientation = so;
}

double ScaraLogic::getFirstSegmentLength() const
{
    return m_firstSegmentLength;
}

double ScaraLogic::getSecondSegmentLength() const
{
    return m_secondSegmentLength;
}

double ScaraLogic::getThirdSegmentLength() const
{
    return m_thirdSegmentLength;
}

double ScaraLogic::getDistanceBetweenTwoLimitsOnZ() const
{
    return m_distanceBetweenTwoLimitsOnZ;
}

double ScaraLogic::getFirstSegmentBeginLimitAngle() const
{
    return m_firstSegmentCWLimitAngle;
}

double ScaraLogic::getFirstSegmentEndLimitAngle() const
{
    return m_firstSegmentCCWLimitAngle;
}

double ScaraLogic::getSecondSegmentBeginLimitAngle() const
{
    return m_secondSegmentCWLimitAngle;
}

double ScaraLogic::getSecondegmentEndLimitAngle() const
{
    return m_secondSegmentCCWLimitAngle;
}

unsigned ScaraLogic::getMotor1maxSteps() const
{
    return m_motor1maxSteps;
}

unsigned ScaraLogic::getMotor2maxSteps() const
{
    return m_motor2maxSteps;
}

unsigned ScaraLogic::getMotor3maxSteps() const
{
    return m_motor3maxSteps;
}


















