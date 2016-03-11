#ifndef STEPPERMOTORRAMP_H
#define STEPPERMOTORRAMP_H


class StepperMotorRamp
{
private:
    unsigned m_startVelocity = 0;
    unsigned m_stopVelocity = 0;
    unsigned m_targetVelocity = 0;
    unsigned m_acceleration = 0;
    unsigned m_deceleration = 0;
    double m_rampDownTime = 0;
    double m_rampUpTime = 0;

public:
    StepperMotorRamp();

    double accumulateTimeForSteps(unsigned steps);

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
