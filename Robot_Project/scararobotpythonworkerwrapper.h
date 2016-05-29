#ifndef SCARAROBOTPYTHONWORKERWRAPPER_H
#define SCARAROBOTPYTHONWORKERWRAPPER_H

#include <QObject>

class ScaraRobotPythonWorkerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ScaraRobotPythonWorkerWrapper(QObject *parent = 0);

signals:

public slots:
};

#endif // SCARAROBOTPYTHONWORKERWRAPPER_H