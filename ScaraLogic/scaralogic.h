#ifndef SCARALOGIC_H
#define SCARALOGIC_H

#include <QtGlobal>
#include <QString>

class Q_DECL_EXPORT ScaraLogic
{
private:
    const double MIN_FIRST_SEGMENT_LENGTH = 10;
    const double MIN_SECOND_SEGMENT_LENGTH = 10;
    const double MIN_THIRD_SEGMENT_LENGTH = 10;

    double m_firstSegmentLength;
    double m_secondSegmentLength;
    double m_thirdSegmentLength;

    double m_firstSegmentBeginLimitAngle;
    double m_firstSegmentEndLimitAngle;

    double m_secondSegmentBeginLimitAngle;
    double m_secondSegmentEndLimitAngle;

    unsigned m_syncFreq;
    unsigned m_motor1maxFreq;
    unsigned m_motor2maxFreq;
    unsigned m_motor3maxFreq;
    unsigned m_motor1maxSteps;
    unsigned m_motor2maxSteps;
    unsigned m_motor3maxSteps;

    double m_motor1anglePerStep;
    double m_motor2anglePerStep;
    double m_motor3distPerStep;

    double m_motor1timePerStep;
    double m_motor2timePerStep;
    double m_motor3timePerStep;

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

    void setFirstSegmentLength(double length);
    void setSecondSegmentLength(double length);
    void setThirdSegmentLength(double length);

    void setFirstSegmentBeginLimitAngle(double angle);
    void setFirstSegmentEndLimitAngle(double angle);

    void setSecondSegmentBeginLimitAngle(double angle);
    void setSecondSegmentEndLimitAngle(double angle);

    void setSyncFreq(unsigned freq);

    void setMotor1maxFreq(unsigned freq);
    void setMotor2maxFreq(unsigned freq);
    void setMotor3maxFreq(unsigned freq);

    void setMotor1maxSteps(unsigned steps);
    void setMotor2maxSteps(unsigned steps);
    void setMotor3maxSteps(unsigned steps);


    double getFirstSegmentLength() const;
    double getSecondSegmentLength() const;
    double getThirdSegmentLength() const;

    double getFirstSegmentBeginLimitAngle() const;
    double getFirstSegmentEndLimitAngle() const;

    double getSecondSegmentBeginLimitAngle() const;
    double getSecondegmentEndLimitAngle() const;

    unsigned getSyncFreq() const;

    unsigned getMotor1maxFreq() const;
    unsigned getMotor2maxFreq() const;
    unsigned getMotor3maxFreq() const;

    unsigned getMotor1maxSteps() const;
    unsigned getMotor2maxSteps() const;
    unsigned getMotor3maxSteps() const;
};

#endif // SCARALOGIC_H

















