#ifndef STEPPERMOTORRAMP_H
#define STEPPERMOTORRAMP_H

#include <QObject>


class Q_DECL_EXPORT StepperMotorRamp : public QObject
{
    Q_OBJECT

private:
    unsigned m_startVelocity = 0;
    unsigned m_stopVelocity = 0;
    unsigned m_targetVelocity = 0;
    unsigned m_acceleration = 0;
    unsigned m_deceleration = 0;
    unsigned m_accumulatedSteps = 0;
    unsigned m_accumulatedTime = 0;
    double m_rampDownTime = 0;
    double m_rampUpTime = 0;

public:
    StepperMotorRamp(QObject* parent=0);

    StepperMotorRamp& operator ++ ();
    StepperMotorRamp& operator -- ();

    double accumulatedTime();

    void setStartVelovity(unsigned velocity);
    void setStopVelocity(unsigned velocity);
    void setTargetVelocity(unsigned velocity);
    void setAcceleration(unsigned acceleration);
    void setDeceleration(unsigned deceleration);
    void setRampDownTime(double time);
    void setRampUpTime(double time);

    unsigned getStartVelocity() const;
    unsigned getStopVelovity() const;
    unsigned getMaxiumumVelocity() const;
    unsigned getAcceleration() const;
    unsigned getDeceleration() const;
    double getRampDownTime() const;
    double getRampUpTime() const;
};

#endif // STEPPERMOTORRAMP_H
