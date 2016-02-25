#include "project.h"

#include <QDebug>
#include <QDomDocument>


Project::Project(QObject *parent)
    : QObject(parent)
{
    m_projectState = Idle;
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

    if ( root.attribute("project_type") == "SCARA - SC1" )
    {
        m_scaraLogic = new ScaraLogic();
        QDomElement element;

        element = com.namedItem("LengthOfFirstSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentLength(element.text());
        else return false;

        element = com.namedItem("LengthOfSecondSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setSecondSegmentLength(element.text());
        else return false;

        element = com.namedItem("CorrectionValue").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setCorrectionValue(element.text());
        else return false;

        element = com.namedItem("FirstSegmentAngleOnCw").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentCWLimitAngle(element.text());
        else return false;

        element = com.namedItem("FirstSegmentAngleOnCCW").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentCCWLimitAngle(element.text());
        else return false;

        element = com.namedItem("NumberOfStepsForFirstSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->set(element.text());
        else return false;

        element = com.namedItem("SecondSegmentAngleOnCw").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentLength(element.text());
        else return false;

        element = com.namedItem("NumberOfStepsForSecondSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentLength(element.text());
        else return false;

        element = com.namedItem("DistanceBetweenTwoLimtsOnZ").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentLength(element.text());
        else return false;

        element = com.namedItem("NumberOfStepsBetweenLimitsOnZ").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentLength(element.text());
        else return false;

        element = com.namedItem("FirstSegmentAngleOnCCW").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentLength(element.text());
        else return false;
    }
    else
        return false;

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
    else
        return false;

    return true;
}

Project::ProjectState Project::projectState()
{
    return m_projectState;
}

void Project::setProjectState(Project::ProjectState const& state)
{
    m_projectState = state;
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


















