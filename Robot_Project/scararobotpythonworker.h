#ifndef SCARAROBOTPYTHONWORKER_H
#define SCARAROBOTPYTHONWORKER_H

#include <QObject>

#include "ethernetcommunicationwidget.h"
#include "scaralogic.h"

class ScaraRobotPythonWorker : public QObject
{
    Q_OBJECT
private:
    EthernetCommunicationWidget* m_communicator;
    ScaraLogic* m_logic;

public:
    explicit ScaraRobotPythonWorker(QObject *parent = 0);


    void homing();
    void moveXYSegmentAngles(double firstSegmentAngle, double secondSegmentAngle);

    void setCommuncator(EthernetCommunicationWidget* communicator);
    void setLogic(ScaraLogic* logic);

signals:

public slots:
};

#endif // SCARAROBOTPYTHONWORKER_H
