#include "scaralogic.h"
#include <cmath>
#include <QObject>
#include <algorithm>

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

QStringList ScaraLogic::moveToPoint(double x, double y, double z, double e)
{
    QStringList commands;
    double l_x = computeXCoordinate(m_firstSegmentAngle, m_secondSegmentAngle);
    double l_y = computeYCoordinate(m_firstSegmentAngle, m_secondSegmentAngle);
    double l_firstSegmentPosInSteps = m_firstSegmentPosInSteps;
    double l_secondSegmentPosInSteps = m_secondSegmentPosInSteps;

    while ( hypot(l_x-x, l_y-y) > e ) // test case
    {
        // Steps after first segment move left
        int safsml = l_firstSegmentPosInSteps - 1;
        // Steps after first segment move right
        int safsmr = l_firstSegmentPosInSteps + 1;
        // Setps after second segment move left
        int sassml = l_secondSegmentPosInSteps - 1;
        // Steps after second segment move right
        int sassmr = l_secondSegmentPosInSteps + 1;

       double safsml_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(safsml),
                                            computeSecondSegmentAngleByStepsPosition(l_secondSegmentPosInSteps));
       double safsml_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(safsml),
                                            computeSecondSegmentAngleByStepsPosition(l_secondSegmentPosInSteps));
       double safsml_distanceToTarget = hypot(safsml_x-x,safsml_y-y);


       double safsmr_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(safsmr),
                                            computeSecondSegmentAngleByStepsPosition(l_secondSegmentPosInSteps));
       double safsmr_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(safsmr),
                                            computeSecondSegmentAngleByStepsPosition(l_secondSegmentPosInSteps));
       double safsmr_distanceToTarget = hypot(safsmr_x-x,safsmr_y-y);


       double sassml_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(l_firstSegmentPosInSteps),
                                            computeSecondSegmentAngleByStepsPosition(sassml));
       double sassml_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(l_firstSegmentPosInSteps),
                                            computeSecondSegmentAngleByStepsPosition(sassml));
       double sassml_distanceToTarget = hypot(sassml_x-x,sassml_y-y);


       double sassmr_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(l_firstSegmentPosInSteps),
                                            computeSecondSegmentAngleByStepsPosition(sassmr));
       double sassmr_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(l_firstSegmentPosInSteps),
                                            computeSecondSegmentAngleByStepsPosition(sassmr));
       double sassmr_distanceToTarget = hypot(sassmr_x-x,sassmr_y-y);


       QList<double> distances;

       distances.append(safsml_distanceToTarget);
       distances.append(safsmr_distanceToTarget);
       distances.append(sassml_distanceToTarget);
       distances.append(sassmr_distanceToTarget);

       double min = *std::min_element(distances.begin(),distances.end());

       if ( min == safsml_distanceToTarget )
           l_firstSegmentPosInSteps = safsml;
       else if ( min == safsmr_distanceToTarget)
           l_firstSegmentPosInSteps = safsmr;
       else if ( min == sassml_distanceToTarget )
           l_secondSegmentPosInSteps = sassml;
       else if ( min == sassmr_distanceToTarget )
           l_secondSegmentPosInSteps = sassmr;

       l_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(l_firstSegmentPosInSteps),
                                computeSecondSegmentAngleByStepsPosition(l_secondSegmentPosInSteps));
       l_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(l_firstSegmentPosInSteps),
                                computeSecondSegmentAngleByStepsPosition(l_secondSegmentPosInSteps));

       int t = 0;
    }

    commands.append(firstSegmentAbsoluteMoveCommand+QString::number(l_firstSegmentPosInSteps));
    commands.append(secondSegmentAbsoluteMoveCommand+QString::number(l_secondSegmentPosInSteps));

    return commands;
}














