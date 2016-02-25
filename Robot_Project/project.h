#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QRunnable>

#include "ethernetcommunicationwidget.h"
#include "scaralogic.h"

class Q_DECL_EXPORT Project : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum ProjectState
    {
        ControlledByScript,
        ControlledByCommandPrompt,
        ControlledByManualControl,
        Idle,
    };

    Project(QObject *parent = 0);

    bool populateFromString(QString data);
    ProjectState projectState();
    void setProjectState(const ProjectState &state);

    void run();

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

public slots:
    void sendCommandSlot(QString command);
    void establishConnectionSlot();
    void dropConnectionSlot();
};

#endif // PROJECT_H
