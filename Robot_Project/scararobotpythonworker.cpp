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

void ScaraRobotPythonWorker::moveXYSegmentAngles(double firstSegmentAngle, double secondSegmentAngle)
{

}

