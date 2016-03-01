#ifndef ETHERNETCOMMUNICATIONWIDGET_H
#define ETHERNETCOMMUNICATIONWIDGET_H

#include <QtNetwork>
#include <QObject>

class Q_DECL_EXPORT EthernetCommunicationWidget : public QObject
{
    Q_OBJECT

private:
    QTcpSocket* m_socket;

    QString m_address;
    QString m_port;
    unsigned m_wastedBytes;
    unsigned m_commandBytes;

public:
    EthernetCommunicationWidget(QObject* parent=0);

    void setAddress(QString const& address);
    void setPort(QString const& port);
    void setWastedBytes(QString const& wastedBytes);
    void setCommandBytes(QString const& commandByets);

    QString getAddress() const;
    QString getPort() const;

    QString readNonBlocking();

signals:
    void sendInfo(QString info);
    void pushCommand(QString command);

private slots:
    void readyRead();
    void connectedToHost();
    void disconnectedFromHost();

public slots:
    void sendCommand(QString command);
    bool establishConnection();
    void dropConnection();
};

#endif // ETHERNETCOMMUNICATIONWIDGET_H
