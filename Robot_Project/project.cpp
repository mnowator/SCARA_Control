#include "project.h"

#include <QDebug>
#include <QDomDocument>


Project::Project(QObject *parent)
    : QObject(parent)
{

}

bool Project::populateFromString(QString data)
{
    QDomDocument dom;
    QDomElement root, com;
    QString errorStr;

    if ( !dom.setContent(data,false,&errorStr))
        return false;

    root = dom.documentElement();

    if ( root.tagName() != "Project")
    {
        return false;
    }

    com = root.namedItem("CommunicationConfig").toElement();

    if ( com.isNull() )
    {
        return false;
    }

    if ( com.attribute("communication_type") == "Ethernet Communication (TCP/IP)")
    {
        m_ethernetCommunicationWidget = new EthernetCommunicationWidget();
        QDomElement element;

        element = com.namedItem("AddressIP").toElement();
        if ( !element.isNull() )
            m_ethernetCommunicationWidget->setAddress(element.text());
        else return false;

        element = com.namedItem("Port").toElement();
        if ( !element.isNull() )
            m_ethernetCommunicationWidget->setPort(element.text());
        else return false;

        element = com.namedItem("WastedBytes").toElement();
        if ( !element.isNull() )
            m_ethernetCommunicationWidget->setWastedBytes(element.text());
        else return false;

        element = com.namedItem("BytesOnCommand").toElement();
        if ( !element.isNull() )
            m_ethernetCommunicationWidget->setCommandBytes(element.text());
        else return false;

        connect(m_ethernetCommunicationWidget,SIGNAL(sendInfo(QString)),this,SIGNAL(sendProjectInfo(QString)));
        connect(m_ethernetCommunicationWidget,SIGNAL(pushCommand(QString)),this,SIGNAL(receivedCommand(QString)));

        connect(this,SIGNAL(sendCommandSignal(QString)),m_ethernetCommunicationWidget,SLOT(sendCommand(QString)));
        connect(this,SIGNAL(establishConnectionSignal()),m_ethernetCommunicationWidget,SLOT(establishConnection()));
        connect(this,SIGNAL(dropConnectionSignal()),m_ethernetCommunicationWidget,SLOT(dropConnection()));
    }

    return true;
}

Project::ProjectState Project::projectState()
{
    1;
}
void Project::run()
{
    for ( unsigned i=0; i<20; ++i)
    {
        qDebug() << i;

        QThread::currentThread()->msleep(500);
    }
}

void Project::sendCommandSlot(QString command)
{
    emit sendCommandSignal(command);
}

void Project::establishConnectionSlot()
{
    emit establishConnectionSignal();
}

void Project::dropConnectionSlot()
{
    emit dropConnectionSignal();
}


















