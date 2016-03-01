#include "scararobotpythonworker.h"

ScaraRobotPythonWorker::ScaraRobotPythonWorker(QObject *parent) : QObject(parent)
{

}

void ScaraRobotPythonWorker::homing()
{
    bool firstSegmentHomed = false;
    bool secondSegmentHomed = false;
    bool thirdSegmentHomed = false;

    m_communicator->sendCommand("HOMINGM2");
    m_communicator->sendCommand("HOMINGM3");
    m_communicator->sendCommand("HOMINGM4");

    while ( true )
    {
        QString receivedCommand = m_communicator->readNonBlocking();
        QString simplifiedCommand = receivedCommand.simplified();

        qDebug() << receivedCommand;

        simplifiedCommand.replace(" ", "");

        if ( receivedCommand == "HOMING_M2_DONE")
        {
            firstSegmentHomed = true;
        }
        else if ( receivedCommand == "HOMING_M3_DONE" )
        {
            secondSegmentHomed = true;
        }
        else if ( receivedCommand == "HOMING_M4_DONE" )
        {
            thirdSegmentHomed = true;
        }

        if ( firstSegmentHomed && secondSegmentHomed && thirdSegmentHomed )
            break;

        QThread::currentThread()->msleep(2);
    }
}

void ScaraRobotPythonWorker::setCommuncator(EthernetCommunicationWidget *communicator)
{
    m_communicator = communicator;
}

void ScaraRobotPythonWorker::setLogic(ScaraLogic *logic)
{
    m_logic = logic;
}

