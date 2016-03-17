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

void ScaraRobotPythonWorker::testCommands()
{
    QStringList commands;
    int pos = 0;

    for ( unsigned i=0; i<100; ++i)
    {
        pos -= 5;
        commands.append(m_logic->firstSegmentAbsoluteMoveCommand+QString::number(pos));
    }

    foreach ( QString command, commands)
    {
        m_communicator->sendCommand(command);

        while ( true )
        {
            QString receivedCommand = m_communicator->readNonBlocking();

            m_logic->processCommand(receivedCommand);

            if ( m_logic->getFirstSegmentHomingState() == NOT_USED )
            {
                break;
            }
        }
    }
}

void ScaraRobotPythonWorker::moveToPoint(double x, double y, double z)
{
    QStringList commands = m_logic->moveToPoint(x,y,0);

    m_communicator->sendCommand(commands[0]);
    m_communicator->sendCommand(commands[1]);

    forever
    {

        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getFirstSegmentMotorState() == NOT_USED &&
             m_logic->getSecondSegmentMotorState() == NOT_USED)
        {
            break;
        }
    }
}











