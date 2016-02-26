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

class Q_DECL_EXPORT Project : public QObject
{
    Q_OBJECT

public:
    Project(QObject *parent = 0);

    bool populateFromString(QString data);
    ProjectState projectState();
    void setProjectState(const ProjectState &state);

    void setupThread(QThread* thread);

private:
    ProjectState m_projectState;    

    EthernetCommunicationWidget* m_ethernetCommunicationWidget;
    ScaraLogic* m_scaraLogic;

signals:
    void sendProjectInfo(QString info);
    void sendCommandSignal(QString command);
    void receivedCommand(QString command);
    void establishConnectionSignal();
    void dropConnectionSignal();
    void changeState(ProjectState state);

private slots:
    void doWork();

public slots:
    void sendCommandSlot(QString command);
    void establishConnectionSlot();
    void dropConnectionSlot();
};

#endif // PROJECT_H
