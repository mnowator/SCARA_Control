#include "ethernetcommunicationwidget.h"

#include <QDebug>

EthernetCommunicationWidget::EthernetCommunicationWidget(QObject *parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(m_socket,SIGNAL(connected()),this,SLOT(connectedToHost()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(disconnectedFromHost()));
}

void EthernetCommunicationWidget::setAddress(const QString &address)
{
    m_address = address;
}

void EthernetCommunicationWidget::setPort(const QString &port)
{
    m_port = port;
}

void EthernetCommunicationWidget::setWastedBytes(const QString &wastedBytes)
{
    m_wastedBytes = wastedBytes.toUInt(0,10);
}

void EthernetCommunicationWidget::setCommandBytes(const QString &commandByets)
{
    m_commandBytes = commandByets.toUInt(0,10);
}

void EthernetCommunicationWidget::sendCommand(QString command)
{
    if ( m_socket->state() != QAbstractSocket::ConnectedState )
    {
        emit sendInfo(tr("You are not connected."));

        return;
    }

    QString preparedCommand;
    QString wastedBytes;

    for ( unsigned i=0;i<m_wastedBytes;++i)
        wastedBytes += ' ';

    preparedCommand = wastedBytes + command;

    if ( preparedCommand.length() < m_commandBytes )
    {
        unsigned charsToAdd = preparedCommand.length();

        for ( unsigned i=0; i<m_commandBytes - charsToAdd; ++i)
        {
            preparedCommand += ' ';
        }
    }
    else if ( preparedCommand.length() > m_commandBytes )
    {
        preparedCommand = preparedCommand.left(m_commandBytes);
    }

    m_socket->write(preparedCommand.toStdString().c_str());
    m_socket->flush();
}

void EthernetCommunicationWidget::readyRead()
{
    QString data;

    while ( m_socket->bytesAvailable())
        data += m_socket->readAll();

    emit pushCommand(data.right(m_commandBytes-m_wastedBytes));
}

void EthernetCommunicationWidget::connectedToHost()
{
    emit sendInfo(tr("Connected to host."));
}

void EthernetCommunicationWidget::disconnectedFromHost()
{
    emit sendInfo(tr("Disconnected from host."));
}

void EthernetCommunicationWidget::establishConnection()
{
    emit sendInfo(tr("Connecting to host at ")+m_address+":"+m_port+"...");

    m_socket->connectToHost(QHostAddress(m_address), m_port.toUShort(0,10));

    if ( !m_socket->waitForConnected(100) )
        emit sendInfo(tr("Connection has not been established."));
}

void EthernetCommunicationWidget::dropConnection()
{
    if ( m_socket->state() == QAbstractSocket::ConnectedState )
    {
        m_socket->disconnectFromHost();

        if (m_socket->state() == QAbstractSocket::UnconnectedState || m_socket->waitForDisconnected(1000));
    }
    else
        emit sendInfo(tr("You are already disconnected from host."));
}

















