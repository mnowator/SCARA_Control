#ifndef SCARAROBOTPYTHONWORKER_H
#define SCARAROBOTPYTHONWORKER_H

#include <QObject>
#include <QDebug>

#include "ethernetcommunicationwidget.h"
#include "scaralogic.h"

class ScaraRobotPythonWorker : public QObject
{
    Q_OBJECT
private:
    EthernetCommunicationWidget* m_communicator;
    ScaraLogic* m_logic;

public:
    ScaraRobotPythonWorker(EthernetCommunicationWidget* communicator=0, ScaraLogic* logic=0, QObject *parent = 0);

public slots:
    void homing();
    void setSpeadBounderies(unsigned motor, unsigned lowerBound, unsigned upperBound);
    void XYmoveToPoint(double x, double y);

};

#endif // SCARAROBOTPYTHONWORKER_H
