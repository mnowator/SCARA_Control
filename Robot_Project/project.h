#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>

#include "ethernetcommunicationwidget.h"

class Q_DECL_EXPORT Project : public QWidget
{
    Q_OBJECT
public:
    enum ProjectState
    {
        ControlledByScript,
        Idle,
    };

    Project(QWidget *parent = 0);

    bool populateFromString(QString data);
    ProjectState projectState();

private:
    ProjectState m_projectState;

    EthernetCommunicationWidget* m_ethernetCommunicationWidget;

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
