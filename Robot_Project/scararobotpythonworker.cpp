#include "scararobotpythonworker.h"

ScaraRobotPythonWorker::ScaraRobotPythonWorker(EthernetCommunicationWidget *communicator, ScaraLogic *logic, QObject *parent) : QObject(parent),
    m_communicator(communicator),
    m_logic(logic)
{

}

void ScaraRobotPythonWorker::homing()
{
    qDebug() << "homing";

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

void ScaraRobotPythonWorker::setSpeadBounderies(unsigned motor, unsigned lowerBound, unsigned upperBound)
{
    if ( motor == 1 )
    {
        m_logic->setMotor1SpeedBounderies(lowerBound,upperBound);
    }
    else if ( motor == 2 )
    {
        m_logic->setMotor2SpeedBounderies(lowerBound,upperBound);
    }
}

void ScaraRobotPythonWorker::XYmoveToPoint(double x, double y)
{
    QStringList commands = m_logic->XYmoveToPoint(x,y);

    qDebug() << commands;

//    if ( commands.empty() )
//        return;

//    foreach( QString command, commands )
//        m_communicator->sendCommand(command);

//    commands[2].replace(m_logic->firstSegmentAbsoluteMoveCommand,"");
//    int firstSegmentPos = commands[2].toInt();

//    commands[3].replace(m_logic->secondSegmentAbsoluteMoveCommand,"");
//    int secondSegmentPos = commands[3].toInt();

//    while ( true )
//    {
//        QString receivedCommand = m_communicator->readNonBlocking();

//        m_logic->processCommand(receivedCommand);

//        if ( m_logic->getFirstSegmentPosInSteps() == firstSegmentPos &&
//             m_logic->getSecondSegmentPosInSteps() == secondSegmentPos )
//        {
//            break;
//        }
//    }
}











