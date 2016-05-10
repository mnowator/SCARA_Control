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
    QStringList commands;

    commands.append("ABM2-3719");
    commands.append("ABM34447");

    m_communicator->sendCommand(commands[0]);
    m_communicator->sendCommand(commands[1]);

    forever
    {

        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getFirstSegmentPosInSteps() == -3719 &&
             m_logic->getSecondSegmentPosInSteps() == 4447 )
        {
            break;
        }
    }

    m_communicator->sendCommand("ABM4135");
    QThread::currentThread()->msleep(500);

    forever
    {

        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getThirdSegmentPosInSteps() == 135 )
        {
            break;
        }
    }

    m_communicator->sendCommand("ABM427");
    QThread::currentThread()->msleep(500);

    forever
    {

        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getThirdSegmentPosInSteps() == 27 )
        {
            break;
        }
    }









    commands.clear();

    commands.append("ABM2-5598");
    commands.append("ABM31253");

    m_communicator->sendCommand(commands[0]);
    m_communicator->sendCommand(commands[1]);

    forever
    {

        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getFirstSegmentPosInSteps() == -5598 &&
             m_logic->getSecondSegmentPosInSteps() == 1253)
        {
            break;
        }
    }

    m_communicator->sendCommand("ABM4145");
    QThread::currentThread()->msleep(500);

    forever
    {

        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getThirdSegmentPosInSteps() == 145 )
        {
            break;
        }
    }

    m_communicator->sendCommand("ABM427");
    QThread::currentThread()->msleep(350);

    forever
    {

        QString receivedCommand = m_communicator->readNonBlocking();

        m_logic->processCommand(receivedCommand);

        if ( m_logic->getThirdSegmentPosInSteps() == 27 )
        {
            break;
        }
    }
}











