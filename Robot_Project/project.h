#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QRunnable>

#include "ethernetcommunicationwidget.h"
#include "scaralogic.h"

enum ProjectState
{
    ControlledByScript,
    ControlledByCommandPrompt,
    ControlledByManualControl,
    Idle,
};

enum ProjectThreadState
{
    NotRunning,
    Running,
    Paused,
};

class Q_DECL_EXPORT Project : public QObject
{
    Q_OBJECT

public:
    Project(QObject *parent = 0);

    bool populateFromString(QString data);
    ProjectState projectState();
    void setProjectState(const ProjectState &state);

    void setupThread(QThread* thread);
    void setupConnections();

    ProjectThreadState getProjectThreadState() const;

private:
    ProjectState m_projectState = Idle;
    ProjectThreadState m_projectThreadState = NotRunning;

    EthernetCommunicationWidget* m_ethernetCommunicationWidget;
    ScaraLogic* m_scaraLogic;

signals:
    void sendProjectInfo(QString info);
    void sendCommandSignal(QString command);
    void receivedCommand(QString command);
    void establishConnectionSignal();
    void dropConnectionSignal();
    void changeState(ProjectState state);
    void positionHasChanged(double x, double y, double z);
    void firstSegmentHomed();
    void firstSegmentNotHomed();
    void secondSegmentHomed();
    void secondSegmentNotHomed();
    void thirdSegmentHomed();
    void thirdSegmentNotHomed();

private slots:
    void doWork();

public slots:
    void receivedCommandSlot(QString command);
    void sendCommandSlot(QString command);
    void establishConnectionSlot();
    void dropConnectionSlot();
};

#endif // PROJECT_H
