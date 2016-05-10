#ifndef SCARALOGIC_H
#define SCARALOGIC_H

#include <QtGlobal>
#include <QString>
#include <QObject>

#include <steppermotorramp.h>

enum SWITCH_ORIENTATION
{
    CW,
    CCW,
};

enum HOMING_STATE
{
    NOT_HOMED,
    HOMED,
};

enum MOTOR_STATE
{
    IN_USE,
    NOT_USED,
};

class Q_DECL_EXPORT ScaraLogic : public QObject
{
    Q_OBJECT

private:
    const double MIN_FIRST_SEGMENT_LENGTH = 10;
    const double MIN_SECOND_SEGMENT_LENGTH = 10;
    const double MIN_THIRD_SEGMENT_LENGTH = 10;

    double m_x = 0;
    double m_y = 0;
    double m_z = 0;

    int m_firstSegmentPosInSteps = 0;
    int m_secondSegmentPosInSteps = 0;
    int m_thirdSegmentPosInStpes = 0;

    double m_firstSegmentAngle = 0;
    double m_secondSegmentAngle = 0;

    double m_firstSegmentLength;
    double m_secondSegmentLength;
    double m_thirdSegmentLength;
    double m_correctionValue;

    double m_distanceBetweenTwoLimitsOnZ;

    double m_firstSegmentCWLimitAngle;
    double m_firstSegmentCCWLimitAngle;

    double m_secondSegmentCWLimitAngle;
    double m_secondSegmentCCWLimitAngle;

    unsigned m_motor1maxSteps;
    unsigned m_motor2maxSteps;
    unsigned m_motor3maxSteps;

    double m_motor1anglePerStep;
    double m_motor2anglePerStep;
    double m_motor3distPerStep;

    SWITCH_ORIENTATION m_firstSegmentHomingOrientation;
    SWITCH_ORIENTATION m_secondSegmentHomingOrientation;
    
    HOMING_STATE m_firstSegmentHomingState = NOT_HOMED;
    HOMING_STATE m_secondSegmentHomingState = NOT_HOMED;
    HOMING_STATE m_thirdSegmentHomingState = NOT_HOMED;
    
    MOTOR_STATE m_firstSegmentMotorState = NOT_USED;
    MOTOR_STATE m_secondSegmentMotorState = NOT_USED;
    MOTOR_STATE m_thirdSegmentMotorState = NOT_USED;

    double computeFirstSegmentAngleByStepsPosition(int stepsPos);
    double computeSecondSegmentAngleByStepsPosition(int stepsPos);

    double computeXCoordinate(double firstSegmentAngle, double secondSegmentAngle);
    double computeYCoordinate(double firstSegmentAngle, double secondSegmentAngle);

    void computeCartesianPositionByAnglesAndDistance();

public:
    ScaraLogic(QObject* parent=0);

    class Exception
    {
    protected:
        QString m_errorMsg;

    public:
        Exception();
        Exception(QString qStr) :
            m_errorMsg(qStr)
        {

        }

        virtual void setMsg(QString qStr)
        {
            m_errorMsg = qStr;
        }

        virtual QString const& what() const
        {
            return m_errorMsg;
        }
    };
    
    const QString firstSegmentHomingCommand = "HOMINGM2";
    const QString secondSegmentHomingCommand = "HOMINGM3";
    const QString thirdSegmentHomingCommand = "HOMINGM4";

    const QString firstSegmentAbsoluteMoveCommand = "ABM2";
    const QString secondSegmentAbsoluteMoveCommand = "ABM3";
    const QString thirdSegmentAbsoluteMoveCommand = "ABM4";
    
    void processCommand(QString command);

    void computePositionBySteps(unsigned firstMotorSteps, unsigned secondMotorSteps, unsigned thirdMotorSteps);

    void computeAnglePerStepMotor1();
    void computeAnglePerStepMotor2();
    void computeDistancePerStepMotor3();

    void motor1Homed();
    void motor2Homed();
    void motor3Homed();

    void setFirstSegmentLength(double length);
    void setSecondSegmentLength(double length);
    void setThirdSegmentLength(double length);
    void setCorrectionValue(double length);

    void setDistanceBetweenTwoLimitsOnZ(double length);

    void setFirstSegmentCWLimitAngle(double angle);
    void setFirstSegmentCCWLimitAngle(double angle);

    void setSecondSegmentCWLimitAngle(double angle);
    void setSecondSegmentCCWLimitAngle(double angle);

    void setMotor1maxSteps(unsigned steps);
    void setMotor2maxSteps(unsigned steps);
    void setMotor3maxSteps(unsigned steps);

    void setMotor1HomingSwitchOrientation(SWITCH_ORIENTATION so);
    void setMotor2HomingSwitchOrientation(SWITCH_ORIENTATION so);

    double getFirstSegmentLength() const;
    double getSecondSegmentLength() const;
    double getThirdSegmentLength() const;
    double getCorrectionValue() const;

    double getDistanceBetweenTwoLimitsOnZ() const;

    double getFirstSegmentBeginLimitAngle() const;
    double getFirstSegmentEndLimitAngle() const;
    double getSecondSegmentBeginLimitAngle() const;
    double getSecondegmentEndLimitAngle() const;

    unsigned getMotor1maxSteps() const;
    unsigned getMotor2maxSteps() const;
    unsigned getMotor3maxSteps() const;

    int getFirstSegmentPosInSteps() const;
    int getSecondSegmentPosInSteps() const;
    int getThirdSegmentPosInSteps() const;
    
    double getXCoordinate() const;
    double getYCoordinate() const;
    double getZCoordinate() const;
    
    HOMING_STATE getFirstSegmentHomingState() const;
    HOMING_STATE getSecondSegmentHomingState() const;
    HOMING_STATE getThirdSegmentHomingState() const;

    MOTOR_STATE getFirstSegmentMotorState() const;
    MOTOR_STATE getSecondSegmentMotorState() const;
    MOTOR_STATE getThirdSegmentMotorState() const;

    QStringList moveToPoint(double x, double y, double z, double e=0.1);

};

#endif // SCARALOGIC_H

















