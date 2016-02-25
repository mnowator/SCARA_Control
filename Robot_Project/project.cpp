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

        element = root.namedItem("LengthOfFirstSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentLength(element.text().toDouble());
        else return false;

        element = root.namedItem("LengthOfSecondSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setSecondSegmentLength(element.text().toDouble());
        else return false;

        element = root.namedItem("CorrectionValue").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setCorrectionValue(element.text().toDouble());
        else return false;

        element = root.namedItem("FirstSegmentAngleOnCw").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentCWLimitAngle(element.text().toDouble());
        else return false;

        element = root.namedItem("FirstSegmentAngleOnCCW").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentCCWLimitAngle(element.text().toDouble());
        else return false;

        element = root.namedItem("NumberOfStepsForFirstSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setMotor1maxSteps(element.text().toUInt());
        else return false;

        element = root.namedItem("SecondSegmentAngleOnCw").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setSecondSegmentCWLimitAngle(element.text().toDouble());
        else return false;

        element = root.namedItem("SecondSegmentAngleOnCCW").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setSecondSegmentCCWLimitAngle(element.text().toDouble());
        else return false;

        element = root.namedItem("NumberOfStepsForSecondSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setMotor2maxSteps(element.text().toUInt());
        else return false;

        element = root.namedItem("LengthOfThirdSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setThirdSegmentLength(element.text().toDouble());
        else return false;

        element = root.namedItem("DistanceBetweenTwoLimtsOnZ").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setDistanceBetweenTwoLimitsOnZ(element.text().toDouble());
        else return false;

        element = root.namedItem("NumberOfStepsBetweenLimitsOnZ").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setMotor3maxSteps(element.text().toDouble());
        else return false;

//        element = root.namedItem("FirstSegmentBeginOn").toElement();
//        if ( !element.isNull() )
//        {
//            if ( element.text() == "CW" )
//            {
//                m_scaraLogic->setMotor1HomingSwitchOrientation(ScaraLogic::SWITCH_ORIENTATION::CW);
//            }
//            else if ( element.text() == "CCW" )
//            {
//                m_scaraLogic->setMotor1HomingSwitchOrientation(ScaraLogic::SWITCH_ORIENTATION::CCW);
//            }
//        }
//        else return false;

//        element = root.namedItem("SecondSegmentBeginOn").toElement();
//        if ( !element.isNull() )
//        {
//            if ( element.text() == "CW" )
//            {
//                m_scaraLogic->setMotor2HomingSwitchOrientation(ScaraLogic::SWITCH_ORIENTATION::CW);
//            }
//            else if ( element.text() == "CCW" )
//            {
//                m_scaraLogic->setMotor2HomingSwitchOrientation(ScaraLogic::SWITCH_ORIENTATION::CCW);
//            }
//        }
//        else return false;

//        m_scaraLogic->computeAnglePerStepMotor1();
//        m_scaraLogic->computeAnglePerStepMotor2();
//        m_scaraLogic->computeDistancePerStepMotor3();
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
    m_ethernetCommunicationWidget->establishConnection();

    for ( unsigned i=0; i<5; ++i)
    {
        qDebug() << i;

        m_ethernetCommunicationWidget->sendCommand("ABM2-3785");
        m_ethernetCommunicationWidget->sendCommand("ABM33860");
        QThread::currentThread()->msleep(1000);
        m_ethernetCommunicationWidget->sendCommand("ABM4-757");

        QThread::currentThread()->msleep(5000);

        m_ethernetCommunicationWidget->sendCommand("ABM2-7866");
        m_ethernetCommunicationWidget->sendCommand("ABM313157");

        QThread::currentThread()->msleep(2200);
        m_ethernetCommunicationWidget->sendCommand("ABM4-940");

        QThread::currentThread()->msleep(10000);
    }

    m_ethernetCommunicationWidget->dropConnection();
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


















