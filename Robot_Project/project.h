#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>

#include "ethernetcommunicationwidget.h"

class Q_DECL_EXPORT Project : public QWidget
{
    Q_OBJECT
private:
    EthernetCommunicationWidget* m_ethernetCommunicationWidget;

public:
    Project(QWidget *parent = 0);

    bool populateFromString(QString data);

signals:
    void sendProjectInfo(QString info);
    void sendCommandSignal(QString command);
    void receivedCommand(QString command);
    void establishConnectionSignal();
    void dropConnectionSignal();

public slots:
    void sendCommandSlot(QString command);
    void establishConnectionSlot();
    void dropConnectionSlot();

};

#endif // PROJECT_H
