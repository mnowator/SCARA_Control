#include "scaralogic.h"
#include <cmath>
#include <QObject>

ScaraLogic::ScaraLogic()
{
}

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

    if ( m_motor3maxSteps != 0 )
        m_motor3distPerStep = m_thirdSegmentLength / m_motor3maxSteps;
}

void ScaraLogic::setCorrectionValue(double length)
{
    m_correctionValue = length;
}

void ScaraLogic::setFirstSegmentCWLimitAngle(double angle)
{
    m_firstSegmentBeginLimitAngle = std::fmod(angle, 360.0);

    if (m_motor1maxSteps !=0)
        m_motor1anglePerStep = std::abs(m_firstSegmentBeginLimitAngle-m_firstSegmentEndLimitAngle)/m_motor1maxSteps;
}

void ScaraLogic::setFirstSegmentCCWLimitAngle(double angle)
{
    m_firstSegmentEndLimitAngle = std::fmod(angle, 360.0);

    if (m_motor1maxSteps !=0)
        m_motor1anglePerStep = std::abs(m_firstSegmentBeginLimitAngle-m_firstSegmentEndLimitAngle)/m_motor1maxSteps;
}

void ScaraLogic::setSecondSegmentCWLimitAngle(double angle)
{
    m_secondSegmentBeginLimitAngle = std::fmod(angle, 360.0);

    if (m_motor2maxSteps !=0)
        m_motor2anglePerStep = std::abs(m_secondSegmentBeginLimitAngle-m_secondSegmentEndLimitAngle)/m_motor2maxSteps;
}

void ScaraLogic::setSecondSegmentCCWLimitAngle(double angle)
{
    m_secondSegmentEndLimitAngle = std::fmod(angle, 360.0);

    if (m_motor2maxSteps !=0)
        m_motor2anglePerStep = std::abs(m_secondSegmentBeginLimitAngle-m_secondSegmentEndLimitAngle)/m_motor2maxSteps;
}

void ScaraLogic::setSyncFreq(unsigned freq)
{
    if ( freq == 0 )
        throw Exception(QObject::tr("Sync frequency cannot be equal to 0."));

    m_syncFreq = freq;
}

void ScaraLogic::setMotor1maxFreq(unsigned freq)
{
    if ( freq == 0 )
        throw Exception(QObject::tr("Motor1 frequency cannot be equal to 0."));

    m_motor1maxFreq = freq;
    m_motor1timePerStep = 1/m_motor1maxFreq;
}

void ScaraLogic::setMotor2maxFreq(unsigned freq)
{
    if ( freq == 0 )
        throw Exception(QObject::tr("Motor2 frequency cannot be equal to 0."));

    m_motor2maxFreq = freq;
    m_motor1timePerStep = 1/m_motor1maxFreq;
}

void ScaraLogic::setMotor3maxFreq(unsigned freq)
{
    if ( freq == 0 )
        throw Exception(QObject::tr("Motor3 frequency cannot be equal to 0."));

    m_motor3maxFreq = freq;
    m_motor1timePerStep = 1/m_motor1maxFreq;
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

double ScaraLogic::getFirstSegmentBeginLimitAngle() const
{
    return m_firstSegmentBeginLimitAngle;
}

double ScaraLogic::getFirstSegmentEndLimitAngle() const
{
    return m_firstSegmentEndLimitAngle;
}

double ScaraLogic::getSecondSegmentBeginLimitAngle() const
{
    return m_secondSegmentBeginLimitAngle;
}

double ScaraLogic::getSecondegmentEndLimitAngle() const
{
    return m_secondSegmentEndLimitAngle;
}

unsigned ScaraLogic::getSyncFreq() const
{
    return m_syncFreq;
}

unsigned ScaraLogic::getMotor1maxFreq() const
{
    return m_motor1maxFreq;
}

unsigned ScaraLogic::getMotor2maxFreq() const
{
    return m_motor2maxFreq;
}

unsigned ScaraLogic::getMotor3maxFreq() const
{
    return m_motor3maxFreq;
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


















