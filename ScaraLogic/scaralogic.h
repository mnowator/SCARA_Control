#ifndef SCARALOGIC_H
#define SCARALOGIC_H

#include <QtGlobal>
#include <QString>

class Q_DECL_EXPORT ScaraLogic
{
public:
    enum SWITCH_ORIENTATION
    {
        CW,
        CCW,
    };
private:
    const double MIN_FIRST_SEGMENT_LENGTH = 10;
    const double MIN_SECOND_SEGMENT_LENGTH = 10;
    const double MIN_THIRD_SEGMENT_LENGTH = 10;

    double m_x;
    double m_y;
    double m_z;

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

    SWITCH_ORIENTATION m_motor1SwitchOrientation;
    SWITCH_ORIENTATION m_motor2SwitchOrientation;

public:
    ScaraLogic();

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

    void computePositionBySteps(unsigned firstMotorSteps, unsigned secondMotorSteps, unsigned thirdMotorSteps);

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


};

#endif // SCARALOGIC_H

















