#include "scaralogic.h"
#include <cmath>
#include <QObject>
#include <algorithm>
#include <queue>
#include <QTime>
#include <QFile>
#include <QDataStream>

#include "node.h"

#define PI 3.14159265

#include <QDebug>

double ScaraLogic::computeFirstSegmentAngleByStepsPosition(int stepsPos)
{
    double angle;

    if ( m_firstSegmentHomingOrientation == CW )
    {
        angle = 90+m_firstSegmentCWLimitAngle+(stepsPos*m_motor1anglePerStep);
    }
    else if ( m_firstSegmentHomingOrientation == CCW )
    {
        angle = -90-m_firstSegmentCCWLimitAngle+(stepsPos*m_motor1anglePerStep);
    }

    return angle;
}

double ScaraLogic::computeSecondSegmentAngleByStepsPosition(int stepsPos)
{
    double angle;

    if ( m_secondSegmentHomingOrientation == CW )
    {
        angle = 90+m_secondSegmentCWLimitAngle+(stepsPos*m_motor2anglePerStep);
    }
    else if ( m_secondSegmentHomingOrientation == CCW )
    {
        angle = -90-m_secondSegmentCCWLimitAngle+(stepsPos*m_motor2anglePerStep);
    }

    return angle;
}

double ScaraLogic::computeXCoordinate(double firstSegmentAngle, double secondSegmentAngle)
{    
    return m_firstSegmentLength*sin(firstSegmentAngle*PI/180)+
            m_secondSegmentLength*sin((firstSegmentAngle+secondSegmentAngle)*PI/180);
}

double ScaraLogic::computeYCoordinate(double firstSegmentAngle, double secondSegmentAngle)
{
    return m_firstSegmentLength*cos(firstSegmentAngle*PI/180)+
            m_secondSegmentLength*cos((firstSegmentAngle+secondSegmentAngle)*PI/180);
}

void ScaraLogic::computeCartesianPositionByAnglesAndDistance()
{
    m_x = computeXCoordinate(m_firstSegmentAngle,m_secondSegmentAngle);
    m_y = computeYCoordinate(m_firstSegmentAngle,m_secondSegmentAngle);
}

ScaraLogic::ScaraLogic(QObject *parent)
    : QObject(parent)
{
}

void ScaraLogic::processCommand(QString command)
{
    QStringList dataList = command.split("|");

    if ( dataList.length() < 7 )
    {
        qDebug() << "Too few elements in received command.";
        return;
    }

    QStringList positions = dataList[6].split(" ");
    positions.removeAll("");

    if ( positions.length() < 3 )
    {
        qDebug() << "Too few positions.";
        return;
    }

    // First segment homing state
    if ( dataList[0].replace(" ", "") == "0" ) // not homed
    {
        m_firstSegmentHomingState = NOT_HOMED;
    }
    else if ( dataList[0].replace(" ", "") == "1" ) // homed
    {
        m_firstSegmentHomingState = HOMED;
    }

    // Second segment homing state
    if ( dataList[1] == "0" ) // not homed
    {
        m_secondSegmentHomingState = NOT_HOMED;
    }
    else if ( dataList[1] == "1" ) // homed
    {
        m_secondSegmentHomingState = HOMED;
    }

    // Third segment homing state
    if ( dataList[2] == "0" ) // not homed
    {
        m_thirdSegmentHomingState = NOT_HOMED;
    }
    else if ( dataList[2] == "1" ) // homed
    {
        m_thirdSegmentHomingState = HOMED;
    }


    // First segment motor state
    if ( dataList[3] == "0" ) // in use
    {
        m_firstSegmentMotorState = IN_USE;
    }
    else if ( dataList[3] == "1" ) // not used
    {
        m_firstSegmentMotorState = NOT_USED;
    }

    // Second segment motor state
    if ( dataList[4] == "0" ) // in use
    {
        m_secondSegmentMotorState = IN_USE;
    }
    else if ( dataList[4] == "1" ) // not used
    {
        m_secondSegmentMotorState = NOT_USED;
    }

    // Third segment motor state
    if ( dataList[5] == "0" ) // in use
    {
        m_thirdSegmentMotorState = IN_USE;
    }
    else if ( dataList[5] == "1" ) // not used
    {
        m_thirdSegmentMotorState = NOT_USED;
    }

    m_firstSegmentPosInSteps = positions[0].toInt();
    m_secondSegmentPosInSteps = positions[1].toInt();
    m_thirdSegmentPosInStpes = positions[2].toInt();

    m_firstSegmentAngle = computeFirstSegmentAngleByStepsPosition( positions[0].toInt() );
    m_secondSegmentAngle = computeSecondSegmentAngleByStepsPosition( positions[1].toInt() );

    computeCartesianPositionByAnglesAndDistance();
}


void ScaraLogic::computePositionBySteps(unsigned firstMotorSteps, unsigned secondMotorSteps, unsigned thirdMotorSteps)
{
}

void ScaraLogic::computeAnglePerStepMotor1()
{
    m_motor1anglePerStep = (180+m_firstSegmentCWLimitAngle+m_firstSegmentCCWLimitAngle)/m_motor1maxSteps;
}

void ScaraLogic::computeAnglePerStepMotor2()
{
    m_motor2anglePerStep = (180+m_secondSegmentCWLimitAngle+m_secondSegmentCCWLimitAngle)/m_motor2maxSteps;
}

void ScaraLogic::computeDistancePerStepMotor3()
{
    m_motor3distPerStep = (m_thirdSegmentLength-m_distanceBetweenTwoLimitsOnZ)/m_motor3maxSteps;
}

void ScaraLogic::motor1Homed()
{
    if ( m_firstSegmentHomingOrientation == CW )
        m_firstSegmentAngle = m_firstSegmentCWLimitAngle+90;
    else if ( m_firstSegmentHomingOrientation == CCW )
        m_firstSegmentAngle = -m_firstSegmentCCWLimitAngle-90;

    computeCartesianPositionByAnglesAndDistance();
}

void ScaraLogic::motor2Homed()
{
    if ( m_secondSegmentHomingOrientation == CW )
        m_secondSegmentAngle = m_secondSegmentCWLimitAngle+90;
    else if ( m_secondSegmentHomingOrientation == CCW )
        m_secondSegmentAngle = -m_secondSegmentCCWLimitAngle-90;

    computeCartesianPositionByAnglesAndDistance();
}

void ScaraLogic::motor3Homed()
{
    m_z = 0;
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

void ScaraLogic::setMotor1HomingSwitchOrientation(SWITCH_ORIENTATION so)
{
    m_firstSegmentHomingOrientation = so;
}

void ScaraLogic::setMotor2HomingSwitchOrientation(SWITCH_ORIENTATION so)
{
    m_secondSegmentHomingOrientation = so;
}

void ScaraLogic::setMotor1SpeedBounderies(unsigned lowerBound, unsigned upperBound)
{
    m_motor1lowerSpeedBound = lowerBound;
    m_motor1upperSpeedBound = upperBound;
}

void ScaraLogic::setMotor2SpeedBounderies(unsigned lowerBound, unsigned upperBound)
{
    m_motor2upperSpeedBound = upperBound;
    m_motor2lowerSpeedBound = lowerBound;
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

int ScaraLogic::getFirstSegmentPosInSteps() const
{
    return m_firstSegmentPosInSteps;
}

int ScaraLogic::getSecondSegmentPosInSteps() const
{
    return m_secondSegmentPosInSteps;
}

int ScaraLogic::getThirdSegmentPosInSteps() const
{
    return m_thirdSegmentPosInStpes;
}

double ScaraLogic::getXCoordinate() const
{
    return m_x;
}

double ScaraLogic::getYCoordinate() const
{
    return m_y;
}

double ScaraLogic::getZCoordinate() const
{
    return m_z;
}

HOMING_STATE ScaraLogic::getFirstSegmentHomingState() const
{
    return m_firstSegmentHomingState;
}

HOMING_STATE ScaraLogic::getSecondSegmentHomingState() const
{
    return m_secondSegmentHomingState;
}

HOMING_STATE ScaraLogic::getThirdSegmentHomingState() const
{
    return m_thirdSegmentHomingState;
}

MOTOR_STATE ScaraLogic::getFirstSegmentMotorState() const
{
    return m_firstSegmentMotorState;
}

MOTOR_STATE ScaraLogic::getSecondSegmentMotorState() const
{
    return m_secondSegmentMotorState;
}

MOTOR_STATE ScaraLogic::getThirdSegmentMotorState() const
{
    return m_thirdSegmentMotorState;
}

QStringList ScaraLogic::XYmoveBySteps(int firstSegmentSteps, int secondSegmentSteps)
{
    QStringList commands;

    firstSegmentSteps = 2000;
    secondSegmentSteps = 2666;

    unsigned firstSegmentAppropierateSpeed;
    unsigned secondSegmentAppropierateSpeed;

    unsigned firstSegmentSteps2Do = std::abs(m_firstSegmentPosInSteps-firstSegmentSteps);
    unsigned secondSegmentSteps2Do = std::abs(m_secondSegmentPosInSteps-secondSegmentSteps);

    double y1 = firstSegmentSteps2Do/(double)m_motor1lowerSpeedBound;
    double x1 = m_motor1lowerSpeedBound;

    double y2 = firstSegmentSteps2Do/(double)m_motor1upperSpeedBound;
    double x2 = m_motor1upperSpeedBound;

    double y3 = secondSegmentSteps2Do/(double)m_motor2lowerSpeedBound;
    double x3 = m_motor2lowerSpeedBound;

    double y4 = secondSegmentSteps2Do/(double)m_motor2upperSpeedBound;
    double x4 = m_motor2upperSpeedBound;

    if ( y2 < y4 ) // first is faster
    {
        secondSegmentAppropierateSpeed = m_motor2upperSpeedBound;
        firstSegmentAppropierateSpeed = firstSegmentSteps2Do/y4;

        if ( firstSegmentAppropierateSpeed < m_motor1lowerSpeedBound)
            firstSegmentAppropierateSpeed = m_motor1lowerSpeedBound;
    }
    else if ( y2 > y4 ) // second is faster
    {
        firstSegmentAppropierateSpeed = m_motor1upperSpeedBound;
        secondSegmentAppropierateSpeed = secondSegmentSteps2Do/y2;

        if ( secondSegmentAppropierateSpeed < m_motor2lowerSpeedBound )
            secondSegmentAppropierateSpeed = m_motor2lowerSpeedBound;
    }

//    double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
//    double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
//    double x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
//    double y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

//    if ( x < std::min(x1, x3) || x > std::max(x1, x2) ||
//        x < std::min(x3, x4) || x > std::max(x3, x4) )
//    {
//        firstSegmentAppropierateSpeed = std::min( m_motor1upperSpeedBound, m_motor2upperSpeedBound);
//        secondSegmentAppropierateSpeed = std::min( m_motor1upperSpeedBound, m_motor2upperSpeedBound);
//    }
//    else
//    {
//        firstSegmentAppropierateSpeed = firstSegmentSteps2Do/y;
//        secondSegmentAppropierateSpeed = secondSegmentSteps2Do/y;
//    }

//    qDebug() << x1 << y1;
//    qDebug() << x2 << y2;
//    qDebug() << x3 << y3;
//    qDebug() << x4 << y4;

//    qDebug() << x << y;

    commands.append(firstSegmentSpeedCommand+QString::number(firstSegmentAppropierateSpeed));
    commands.append(secondSegmentSpeedCommand+QString::number(secondSegmentAppropierateSpeed));
    commands.append(firstSegmentAbsoluteMoveCommand+QString::number(firstSegmentSteps));
    commands.append(secondSegmentAbsoluteMoveCommand+QString::number(secondSegmentSteps));

    return commands;
}

QStringList ScaraLogic::XYmoveToPoint(double x, double y)
{
    double theta2positive = std::acos((std::pow(x,2)+std::pow(y,2)-std::pow(m_firstSegmentLength,2)-std::pow(m_secondSegmentLength,2))/(2*m_firstSegmentLength*m_secondSegmentLength));
    double theta2negative = -std::acos((std::pow(x,2)+std::pow(y,2)-std::pow(m_firstSegmentLength,2)-std::pow(m_secondSegmentLength,2))/(2*m_firstSegmentLength*m_secondSegmentLength));

    if ( theta2positive*180/PI < -m_secondSegmentCWLimitAngle &&
         theta2positive*180/PI > m_secondSegmentCCWLimitAngle+180 )
    {
        theta2positive = NAN;
    }

    if ( theta2negative*180/PI < -m_secondSegmentCWLimitAngle &&
         theta2negative*180/PI > m_secondSegmentCCWLimitAngle+180 )
    {
        theta2negative = NAN;
    }

    double theta1positive = std::atan2(((m_firstSegmentLength+(m_secondSegmentLength*std::cos(theta2positive)))*x) - (m_secondSegmentLength*std::sin(theta2positive)*y),
                                   (m_secondSegmentLength*std::sin(theta2positive)*x)+((m_firstSegmentLength+(m_secondSegmentLength*std::cos(theta2positive)))*y));
    double theta1negative = std::atan2(((m_firstSegmentLength+(m_secondSegmentLength*std::cos(theta2negative)))*x) - (m_secondSegmentLength*std::sin(theta2negative)*y),
                                   (m_secondSegmentLength*std::sin(theta2negative)*x)+((m_firstSegmentLength+(m_secondSegmentLength*std::cos(theta2negative)))*y));

    if ( theta1positive*180/PI < -m_firstSegmentCWLimitAngle &&
         theta1positive*180/PI > m_firstSegmentCCWLimitAngle+180 )
    {
        theta1positive = NAN;
    }

    if ( theta1negative*180/PI < -m_firstSegmentCWLimitAngle &&
         theta1negative*180/PI > m_firstSegmentCCWLimitAngle+180 )
    {
        theta1negative = NAN;
    }


    if ( theta1positive != NAN && std::fabs(m_firstSegmentAngle-theta1positive ) < std::fabs(m_firstSegmentAngle-theta1negative ) )
    {
        // positive solution is better
        return XYmoveByAngles(theta1positive*180/PI, theta2positive*180/PI);
    }
    else if ( theta1negative != NAN && std::fabs(m_firstSegmentAngle-theta1positive ) > std::fabs(m_firstSegmentAngle-theta1negative ))
    {
        // negative solution is better
        return XYmoveByAngles(theta1negative*180/PI, theta2negative*180/PI);
    }

    // there is no solution
    return QStringList();
}

QStringList ScaraLogic::XYmoveByAngles(double theta1, double theta2)
{
    if ( theta1*180/PI < -m_firstSegmentCWLimitAngle &&
         theta1*180/PI > m_firstSegmentCCWLimitAngle+180 )
    {
        return QStringList();
    }

    if ( theta2*180/PI < -m_secondSegmentCWLimitAngle &&
         theta2*180/PI > m_secondSegmentCCWLimitAngle+180 )
    {
        return QStringList();
    }


    int firstSegmentSteps = static_cast<int>( theta1/m_motor1anglePerStep );
    int secondSemgentSteps = static_cast<int>( theta2/m_motor2anglePerStep );

    return XYmoveBySteps(firstSegmentSteps, secondSemgentSteps);
}















