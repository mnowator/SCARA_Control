#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>

class Q_DECL_EXPORT Project : public QWidget
{
    Q_OBJECT
public:
    Project(QWidget *parent = 0);

signals:
    void receivedCommand(QString command);
    void sendProjectInfo(QString info);

public slots:
    void sendCommand(QString command);
};

#endif // PROJECT_H
