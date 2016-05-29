#include "project.h"

#include <QDebug>
#include <QDomDocument>

#include "scararobotpythonworker.h"

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
        m_scaraLogic = new ScaraLogic(this);
        QDomElement element;

        element = root.namedItem("MainProgramFile").toElement();
        if ( !element.isNull() )
        {
            m_scriptFileName = element.text();
        }
        else return false;

        element = root.namedItem("LengthOfFirstSegment").toElement();
        if ( !element.isNull() )
        {
            m_scaraLogic->setFirstSegmentLength(element.text().replace(",",".").toDouble());
        }
        else return false;

        element = root.namedItem("LengthOfSecondSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setSecondSegmentLength(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("CorrectionValue").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setCorrectionValue(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("FirstSegmentAngleOnCw").toElement();
        if ( !element.isNull() )
        {
            m_scaraLogic->setFirstSegmentCWLimitAngle(element.text().replace(",",".").toDouble());
        }
        else return false;

        element = root.namedItem("FirstSegmentAngleOnCCW").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setFirstSegmentCCWLimitAngle(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("NumberOfStepsForFirstSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setMotor1maxSteps(element.text().replace(",",".").toUInt());
        else return false;

        element = root.namedItem("SecondSegmentAngleOnCw").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setSecondSegmentCWLimitAngle(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("SecondSegmentAngleOnCCW").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setSecondSegmentCCWLimitAngle(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("NumberOfStepsForSecondSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setMotor2maxSteps(element.text().replace(",",".").toUInt());
        else return false;

        element = root.namedItem("LengthOfThirdSegment").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setThirdSegmentLength(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("DistanceBetweenTwoLimtsOnZ").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setDistanceBetweenTwoLimitsOnZ(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("NumberOfStepsBetweenLimitsOnZ").toElement();
        if ( !element.isNull() )
            m_scaraLogic->setMotor3maxSteps(element.text().replace(",",".").toDouble());
        else return false;

        element = root.namedItem("FirstSegmentBeginOn").toElement();
        if ( !element.isNull() )
        {
            if ( element.text() == "CW" )
            {
                m_scaraLogic->setMotor1HomingSwitchOrientation(CW);
            }
            else if ( element.text() == "CCW" )
            {
                m_scaraLogic->setMotor1HomingSwitchOrientation(CCW);
            }
        }
        else return false;

        element = root.namedItem("SecondSegmentBeginOn").toElement();
        if ( !element.isNull() )
        {
            if ( element.text() == "CW" )
            {
                m_scaraLogic->setMotor2HomingSwitchOrientation(CW);
            }
            else if ( element.text() == "CCW" )
            {
                m_scaraLogic->setMotor2HomingSwitchOrientation(CCW);
            }
        }
        else return false;

        m_scaraLogic->computeAnglePerStepMotor1();
        m_scaraLogic->computeAnglePerStepMotor2();
        m_scaraLogic->computeDistancePerStepMotor3();
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
        m_ethernetCommunicationWidget = new EthernetCommunicationWidget(this);
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

        connect(m_ethernetCommunicationWidget,SIGNAL(pushCommand(QString)),this,SLOT(receivedCommandSlot(QString)));
        connect(this,SIGNAL(sendCommandSignal(QString)),m_ethernetCommunicationWidget,SLOT(sendCommand(QString)));
        connect(this,SIGNAL(establishConnectionSignal()),m_ethernetCommunicationWidget,SLOT(establishConnection()));
        connect(this,SIGNAL(dropConnectionSignal()),m_ethernetCommunicationWidget,SLOT(dropConnection()));
    }
    else
        return false;

    QDomElement files = root.namedItem("Files").toElement();
    for ( QDomElement file = files.firstChildElement("File"); !file.isNull(); file = file.nextSiblingElement("File") )
    {
        if ( file.namedItem("Name").toElement().text() == m_scriptFileName )
            m_scriptFilePath = file.namedItem("Path").toElement().text();
    }


    return true;
}

ProjectState Project::projectState()
{
    return m_projectState;
}

void Project::setProjectState(ProjectState const& state)
{
    m_projectState = state;
}

void Project::setupThread(QThread *thread)
{
    connect(thread,SIGNAL(started()),this,SLOT(doWork()));
}

void Project::setupConnections()
{
    m_ethernetCommunicationWidget->setConnections();
}

ProjectThreadState Project::getProjectThreadState() const
{
    return m_projectThreadState;
}

#include <QPushButton>
#include <QtScript/QScriptEngine>

void Project::doWork()
{
    QScriptEngine engine;

    ScaraRobotPythonWorker worker(m_ethernetCommunicationWidget, m_scaraLogic);

    QScriptValue scriptWorker = engine.newQObject(&worker);

    engine.globalObject().setProperty("scara",scriptWorker);

    QFile file(m_scriptFilePath+m_scriptFileName);

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream textStream(&file);

        QString code = textStream.readAll();

        QScriptValue result = engine.evaluate(code);

        if ( engine.hasUncaughtException() )
            qDebug() << "blad";
    }
    else
        return;


    qDebug() << "Project thread started...";
    qDebug() << "Thread id: " << QThread::currentThreadId();

    qDebug() << "Connectiong to host...";

    if ( m_ethernetCommunicationWidget->establishConnection() )
    {
        qDebug() << "Connected to host at: "
                 << m_ethernetCommunicationWidget->getAddress()
                 << ":"
                 << m_ethernetCommunicationWidget->getPort()
                 << ".";
    }
    else
    {
        qDebug() << "Couldn't connect to host.";

        qDebug() << "Returing project to main thread.";
        moveToThread(qApp->thread());

        m_projectThreadState = NotRunning;
        qDebug() << "Terminating used thread.";
        QThread::currentThread()->terminate();

        return;
    }


    qDebug() << "Droping connection...";
    m_ethernetCommunicationWidget->dropConnection();
    qDebug() << "Connection droped.";

    qDebug() << "Returing project to main thread.";
    moveToThread(qApp->thread());

    m_projectThreadState = NotRunning;
    qDebug() << "Terminating used thread.";
    QThread::currentThread()->terminate();
}

void Project::receivedCommandSlot(QString command)
{
    m_scaraLogic->processCommand(command);

    if ( m_scaraLogic->getFirstSegmentHomingState() == HOMED )
        emit firstSegmentHomed();
    else
        emit firstSegmentNotHomed();

    if ( m_scaraLogic->getSecondSegmentHomingState() == HOMED )
        emit secondSegmentHomed();
    else
        emit secondSegmentNotHomed();

    if ( m_scaraLogic->getThirdSegmentHomingState() == HOMED )
        emit thirdSegmentHomed();
    else
        emit thirdSegmentNotHomed();

    emit positionHasChanged(m_scaraLogic->getXCoordinate(),m_scaraLogic->getYCoordinate(),m_scaraLogic->getZCoordinate());
    emit receivedCommand(command);
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


















