#ifndef SCARAROBOTPYTHONWORKER_H
#define SCARAROBOTPYTHONWORKER_H

#include <QObject>
#include <QDebug>

#include "ethernetcommunicationwidget.h"
#include "scaralogic.h"

class ScaraRobotWrapper : public QObject
{
    Q_OBJECT
private:
    EthernetCommunicationWidget* m_communicator;
    ScaraLogic* m_logic;

public:
    ScaraRobotWrapper(EthernetCommunicationWidget* communicator=0, ScaraLogic* logic=0, QObject *parent = 0);

public slots:
    void homing();
    void sleep(unsigned msec);
    void setSpeadBounderies(unsigned motor, unsigned lowerBound, unsigned upperBound);
    void setThirdSegmentSeeed(unsigned speed);
    void XYmoveToPoint(double x, double y);
    void ZmoveTo(double distance);
    void pick();
    void place();
    void moveMotorsToPos(int pos1, int pos2, int pos3);
};

#endif
