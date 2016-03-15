#include "scararobotpythonworker.h"

ScaraRobotPythonWorker::ScaraRobotPythonWorker(EthernetCommunicationWidget *communicator, ScaraLogic *logic, QObject *parent) : QObject(parent),
    m_communicator(communicator),
    m_logic(logic)
{

}

void ScaraRobotPythonWorker::homing()
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

void ScaraRobotPythonWorker::moveToPoint(double x, double y, double z)
{
    QStringList commands = m_logic->moveToPoint(x,y,z);

    foreach ( QString command, commands)
        m_communicator->sendCommand(command);

    while ( true )
    {
        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getFirstSegmentMotorState() == NOT_USED &&
             m_logic->getSecondSegmentMotorState() == NOT_USED &&
             m_logic->getThirdSegmentMotorState() == NOT_USED )
        {
            break;
        }
    }
}











