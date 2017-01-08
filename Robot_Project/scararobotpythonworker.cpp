#include "scararobotpythonworker.h"

ScaraRobotWrapper::ScaraRobotWrapper(EthernetCommunicationWidget *communicator, ScaraLogic *logic, QObject *parent) : QObject(parent),
    m_communicator(communicator),
    m_logic(logic)
{

}

void ScaraRobotWrapper::homing()
{
    m_communicator->sendCommand(m_logic->firstSegmentHomingCommand);
    m_communicator->sendCommand(m_logic->secondSegmentHomingCommand);
    m_communicator->sendCommand(m_logic->thirdSegmentHomingCommand);

    while ( true )
    {
        QString receivedCommand = m_communicator->readNonBlocking();
        
        m_logic->processCommand(receivedCommand);

        if ( m_logic->getFirstSegmentHomingState() == HOMED &&
             m_logic->getSecondSegmentHomingState() == HOMED &&
             m_logic->getThirdSegmentHomingState() == HOMED)
        {
            break;
        }
    }
}

void ScaraRobotWrapper::sleep(unsigned msec)
{
    QThread::currentThread()->msleep(msec);
}

void ScaraRobotWrapper::setSpeadBounderies(unsigned motor, unsigned lowerBound, unsigned upperBound)
{
    if ( motor == 1 )
    {
        m_logic->setMotor1SpeedBounderies(lowerBound,upperBound);
    }
    else if ( motor == 2 )
    {
        m_logic->setMotor2SpeedBounderies(lowerBound,upperBound);
    }
    else if ( motor == 3 )
    {
        m_logic->setMotor3SpeedBounderies(lowerBound,upperBound);
    }
}

void ScaraRobotWrapper::setThirdSegmentSeeed(unsigned speed)
{
    m_communicator->sendCommand(m_logic->thirdSegmentSpeedCommand+QString::number(speed));
}

void ScaraRobotWrapper::XYmoveToPoint(double x, double y)
{
    QStringList commands = m_logic->XYmoveToPoint(x,y);

    if ( commands.empty() )
        return;

    foreach( QString command, commands )
        m_communicator->sendCommand(command);

    commands[2].replace(m_logic->firstSegmentAbsoluteMoveCommand,"");
    int firstSegmentPos = commands[2].toInt();

    commands[3].replace(m_logic->secondSegmentAbsoluteMoveCommand,"");
    int secondSegmentPos = commands[3].toInt();

    while ( true )
    {
        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getFirstSegmentPosInSteps() == firstSegmentPos &&
             m_logic->getSecondSegmentPosInSteps() == secondSegmentPos )
        {
            break;
        }
    }
}

void ScaraRobotWrapper::ZmoveTo(double distance)
{
    QStringList commands = m_logic->ZmoveTo(distance);

    qDebug() << commands;

    foreach( QString command, commands )
        m_communicator->sendCommand(command);

    commands[0].replace(m_logic->thirdSegmentAbsoluteMoveCommand,"");
    int stepsToDo = commands[0].toInt();

    while ( true )
    {
        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getThirdSegmentPosInSteps() == stepsToDo )
        {
            break;
        }
    }
}

void ScaraRobotWrapper::pick()
{
    m_communicator->sendCommand(m_logic->pickCommand);
}

void ScaraRobotWrapper::place()
{
    m_communicator->sendCommand(m_logic->placeCommand);
}

void ScaraRobotWrapper::moveMotorsToPos(int pos1, int pos2, int pos3)
{
    QStringList commands = m_logic->moveMotorsToPos(pos1, pos2, pos3);

    foreach( QString command, commands )
        m_communicator->sendCommand(command);

    commands[3].replace(m_logic->firstSegmentAbsoluteMoveCommand,"");
    int firstSegmentPos = commands[3].toInt();

    commands[4].replace(m_logic->secondSegmentAbsoluteMoveCommand,"");
    int secondSegmentPos = commands[4].toInt();

    commands[5].replace(m_logic->secondSegmentAbsoluteMoveCommand,"");
    int thirdSegmentPos = commands[5].toInt();

    while ( true )
    {
        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getFirstSegmentPosInSteps() == firstSegmentPos &&
             m_logic->getSecondSegmentPosInSteps() == secondSegmentPos &&
             m_logic->getThirdSegmentPosInSteps() == thirdSegmentPos)
        {
            break;
        }
    }
}











