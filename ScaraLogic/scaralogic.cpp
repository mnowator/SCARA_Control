#include "scaralogic.h"
#include <cmath>
#include <QObject>
#include <algorithm>
#include <queue>
#include <QTime>
#include <QFile>
#include <QDataStream>

#include "node.h"

#define PI 3.14159265

#include <QDebug>

double ScaraLogic::computeFirstSegmentAngleByStepsPosition(int stepsPos)
{
    double angle;

    if ( m_firstSegmentHomingOrientation == CW )
    {
        angle = 90+m_firstSegmentCWLimitAngle+(stepsPos*m_motor1anglePerStep);
    }
    else if ( m_firstSegmentHomingOrientation == CCW )
    {
        angle = -90-m_firstSegmentCCWLimitAngle+(stepsPos*m_motor1anglePerStep);
    }

    return angle;
}

double ScaraLogic::computeSecondSegmentAngleByStepsPosition(int stepsPos)
{
    double angle;

    if ( m_secondSegmentHomingOrientation == CW )
    {
        angle = 90+m_secondSegmentCWLimitAngle+(stepsPos*m_motor2anglePerStep);
    }
    else if ( m_secondSegmentHomingOrientation == CCW )
    {
        angle = -90-m_secondSegmentCCWLimitAngle+(stepsPos*m_motor2anglePerStep);
    }

    return angle;
}

double ScaraLogic::computeXCoordinate(double firstSegmentAngle, double secondSegmentAngle)
{    
    return m_firstSegmentLength*sin(firstSegmentAngle*PI/180)+
            m_secondSegmentLength*sin((firstSegmentAngle+secondSegmentAngle)*PI/180);
}

double ScaraLogic::computeYCoordinate(double firstSegmentAngle, double secondSegmentAngle)
{
    return m_firstSegmentLength*cos(firstSegmentAngle*PI/180)+
            m_secondSegmentLength*cos((firstSegmentAngle+secondSegmentAngle)*PI/180);
}

void ScaraLogic::computeCartesianPositionByAnglesAndDistance()
{
    m_x = computeXCoordinate(m_firstSegmentAngle,m_secondSegmentAngle);
    m_y = computeYCoordinate(m_firstSegmentAngle,m_secondSegmentAngle);
}

ScaraLogic::ScaraLogic(QObject *parent)
    : QObject(parent)
{
}

void ScaraLogic::processCommand(QString command)
{
    QStringList dataList = command.split("|");

    if ( dataList.length() < 7 )
    {
        qDebug() << "Too few elements in received command.";
        return;
    }

    QStringList positions = dataList[6].split(" ");
    positions.removeAll("");

    if ( positions.length() < 3 )
    {
        qDebug() << "Too few positions.";
        return;
    }

    // First segment homing state
    if ( dataList[0].replace(" ", "") == "0" ) // not homed
    {
        m_firstSegmentHomingState = NOT_HOMED;
    }
    else if ( dataList[0].replace(" ", "") == "1" ) // homed
    {
        m_firstSegmentHomingState = HOMED;
    }

    // Second segment homing state
    if ( dataList[1] == "0" ) // not homed
    {
        m_secondSegmentHomingState = NOT_HOMED;
    }
    else if ( dataList[1] == "1" ) // homed
    {
        m_secondSegmentHomingState = HOMED;
    }

    // Third segment homing state
    if ( dataList[2] == "0" ) // not homed
    {
        m_thirdSegmentHomingState = NOT_HOMED;
    }
    else if ( dataList[2] == "1" ) // homed
    {
        m_thirdSegmentHomingState = HOMED;
    }


    // First segment motor state
    if ( dataList[3] == "0" ) // in use
    {
        m_firstSegmentMotorState = IN_USE;
    }
    else if ( dataList[3] == "1" ) // not used
    {
        m_firstSegmentMotorState = NOT_USED;
    }

    // Second segment motor state
    if ( dataList[4] == "0" ) // in use
    {
        m_secondSegmentMotorState = IN_USE;
    }
    else if ( dataList[4] == "1" ) // not used
    {
        m_secondSegmentMotorState = NOT_USED;
    }

    // Third segment motor state
    if ( dataList[5] == "0" ) // in use
    {
        m_thirdSegmentMotorState = IN_USE;
    }
    else if ( dataList[5] == "1" ) // not used
    {
        m_thirdSegmentMotorState = NOT_USED;
    }

    m_firstSegmentPosInSteps = positions[0].toInt();
    m_secondSegmentPosInSteps = positions[1].toInt();

    m_firstSegmentAngle = computeFirstSegmentAngleByStepsPosition( positions[0].toInt() );
    m_secondSegmentAngle = computeSecondSegmentAngleByStepsPosition( positions[1].toInt() );

    computeCartesianPositionByAnglesAndDistance();
}


void ScaraLogic::computePositionBySteps(unsigned firstMotorSteps, unsigned secondMotorSteps, unsigned thirdMotorSteps)
{
}

void ScaraLogic::computeAnglePerStepMotor1()
{
    m_motor1anglePerStep = (180+m_firstSegmentCWLimitAngle+m_firstSegmentCCWLimitAngle)/m_motor1maxSteps;
}

void ScaraLogic::computeAnglePerStepMotor2()
{
    m_motor2anglePerStep = (180+m_secondSegmentCWLimitAngle+m_secondSegmentCCWLimitAngle)/m_motor2maxSteps;
}

void ScaraLogic::computeDistancePerStepMotor3()
{
    m_motor3distPerStep = (m_thirdSegmentLength-m_distanceBetweenTwoLimitsOnZ)/m_motor3maxSteps;
}

void ScaraLogic::motor1Homed()
{
    if ( m_firstSegmentHomingOrientation == CW )
        m_firstSegmentAngle = m_firstSegmentCWLimitAngle+90;
    else if ( m_firstSegmentHomingOrientation == CCW )
        m_firstSegmentAngle = -m_firstSegmentCCWLimitAngle-90;

    computeCartesianPositionByAnglesAndDistance();
}

void ScaraLogic::motor2Homed()
{
    if ( m_secondSegmentHomingOrientation == CW )
        m_secondSegmentAngle = m_secondSegmentCWLimitAngle+90;
    else if ( m_secondSegmentHomingOrientation == CCW )
        m_secondSegmentAngle = -m_secondSegmentCCWLimitAngle-90;

    computeCartesianPositionByAnglesAndDistance();
}

void ScaraLogic::motor3Homed()
{
    m_z = 0;
}

void ScaraLogic::setFirstSegmentLength(double length)
{
    if ( length < MIN_FIRST_SEGMENT_LENGTH )
        throw Exception(QObject::tr("First segment length is less than min value."));

    m_firstSegmentLength = length;
}

void ScaraLogic::setSecondSegmentLength(double length)
{
    if ( length < MIN_SECOND_SEGMENT_LENGTH )
        throw Exception(QObject::tr("Second segment length is less than min value."));

    m_secondSegmentLength = length;
}

void ScaraLogic::setThirdSegmentLength(double length)
{
    if ( length < MIN_THIRD_SEGMENT_LENGTH )
        throw Exception(QObject::tr("Third segment length is less than min value."));

    m_thirdSegmentLength = length;
}

void ScaraLogic::setCorrectionValue(double length)
{
    m_correctionValue = length;
}

void ScaraLogic::setDistanceBetweenTwoLimitsOnZ(double length)
{
    m_distanceBetweenTwoLimitsOnZ = length;
}

void ScaraLogic::setFirstSegmentCWLimitAngle(double angle)
{
    m_firstSegmentCWLimitAngle = std::fmod(angle, 360.0);
}

void ScaraLogic::setFirstSegmentCCWLimitAngle(double angle)
{
    m_firstSegmentCCWLimitAngle = std::fmod(angle, 360.0);
}

void ScaraLogic::setSecondSegmentCWLimitAngle(double angle)
{
    m_secondSegmentCWLimitAngle = std::fmod(angle, 360.0);;
}

void ScaraLogic::setSecondSegmentCCWLimitAngle(double angle)
{
    m_secondSegmentCCWLimitAngle = std::fmod(angle, 360.0);
}

void ScaraLogic::setMotor1maxSteps(unsigned steps)
{
    m_motor1maxSteps = steps;
}

void ScaraLogic::setMotor2maxSteps(unsigned steps)
{
    m_motor2maxSteps = steps;
}

void ScaraLogic::setMotor3maxSteps(unsigned steps)
{
    m_motor3maxSteps = steps;

}

void ScaraLogic::setMotor1HomingSwitchOrientation(SWITCH_ORIENTATION so)
{
    m_firstSegmentHomingOrientation = so;
}

void ScaraLogic::setMotor2HomingSwitchOrientation(SWITCH_ORIENTATION so)
{
    m_secondSegmentHomingOrientation = so;
}

double ScaraLogic::getFirstSegmentLength() const
{
    return m_firstSegmentLength;
}

double ScaraLogic::getSecondSegmentLength() const
{
    return m_secondSegmentLength;
}

double ScaraLogic::getThirdSegmentLength() const
{
    return m_thirdSegmentLength;
}

double ScaraLogic::getDistanceBetweenTwoLimitsOnZ() const
{
    return m_distanceBetweenTwoLimitsOnZ;
}

double ScaraLogic::getFirstSegmentBeginLimitAngle() const
{
    return m_firstSegmentCWLimitAngle;
}

double ScaraLogic::getFirstSegmentEndLimitAngle() const
{
    return m_firstSegmentCCWLimitAngle;
}

double ScaraLogic::getSecondSegmentBeginLimitAngle() const
{
    return m_secondSegmentCWLimitAngle;
}

double ScaraLogic::getSecondegmentEndLimitAngle() const
{
    return m_secondSegmentCCWLimitAngle;
}

unsigned ScaraLogic::getMotor1maxSteps() const
{
    return m_motor1maxSteps;
}

unsigned ScaraLogic::getMotor2maxSteps() const
{
    return m_motor2maxSteps;
}

unsigned ScaraLogic::getMotor3maxSteps() const
{
    return m_motor3maxSteps;
}

double ScaraLogic::getXCoordinate() const
{
    return m_x;
}

double ScaraLogic::getYCoordinate() const
{
    return m_y;
}

double ScaraLogic::getZCoordinate() const
{
    return m_z;
}

HOMING_STATE ScaraLogic::getFirstSegmentHomingState() const
{
    return m_firstSegmentHomingState;
}

HOMING_STATE ScaraLogic::getSecondSegmentHomingState() const
{
    return m_secondSegmentHomingState;
}

HOMING_STATE ScaraLogic::getThirdSegmentHomingState() const
{
    return m_thirdSegmentHomingState;
}

MOTOR_STATE ScaraLogic::getFirstSegmentMotorState() const
{
    return m_firstSegmentMotorState;
}

MOTOR_STATE ScaraLogic::getSecondSegmentMotorState() const
{
    return m_secondSegmentMotorState;
}

MOTOR_STATE ScaraLogic::getThirdSegmentMotorState() const
{
    return m_thirdSegmentMotorState;
}

QStringList ScaraLogic::moveToPoint(double x, double y, double z, double e)
{
    motor1Homed();
    motor2Homed();
    motor3Homed();

    QTime timer;
    timer.start();

    QFile dataFile("data.dat");
    QTextStream data(&dataFile);

    dataFile.open(QFile::WriteOnly | QFile::Text );


    unsigned bestSolutionFirstSegmentSteps = abs(m_firstSegmentPosInSteps);
    unsigned bestSolutionSecondSegmentSteps = abs(m_secondSegmentPosInSteps);

    double minTime = -1;

    if ( hypot(m_x - x, m_y - y ) <= e )
    {
        qDebug() << "zajebiscie";
    }
    else
    {
        for ( unsigned i=0; i<=m_motor1maxSteps; ++i)
        {
            for ( unsigned j=0; j<=m_motor2maxSteps; ++j )
            {
                int stepsForFirst = m_firstSegmentHomingOrientation == CW ? -i : i;
                int stepsForSecond = m_secondSegmentHomingOrientation == CW ? -j : j;

                double firstSegmentAngle = computeFirstSegmentAngleByStepsPosition(stepsForFirst);
                double secondSegmentAngle = computeSecondSegmentAngleByStepsPosition(stepsForSecond);

                double l_x = computeXCoordinate(firstSegmentAngle, secondSegmentAngle);
                double l_y = computeYCoordinate(firstSegmentAngle, secondSegmentAngle);

                data << stepsForFirst << " " << stepsForSecond << " " << hypot(l_x - x, l_y -y) << " " << QString::number(std::max(abs(stepsForFirst)/1000.0, abs(stepsForSecond)/1000.0)) << "\n";

                if ( hypot(l_x - x, l_y -y) <= e )
                {
                    double time = std::max(abs(stepsForFirst)/1000.0, abs(stepsForSecond)/1000.0);

                    if ( time  < minTime || minTime == -1)
                    {
                        minTime = time;
                        bestSolutionFirstSegmentSteps = i;
                        bestSolutionSecondSegmentSteps = j;
                    }
                }
            }
        }
    }

    dataFile.close();

    //        bool** closedNodes = new bool*[m_motor1maxSteps+1];
    //        double** openNodes = new double*[m_motor1maxSteps+1];

    //        double xStart = m_x;
    //        double yStart = m_y;

    //        std::priority_queue<Node*> nodesPriorityQueue;

    //        for ( unsigned i=0; i<m_motor1maxSteps+1; ++i)
    //        {
    //            closedNodes[i] = new bool[m_motor2maxSteps+1];
    //            openNodes[i] = new double[m_motor2maxSteps+1];

    //            for ( unsigned j=0; j<m_motor2maxSteps+1; ++j)
    //            {
    //                closedNodes[i][j] = false;
    //                openNodes[i][j] = 0;
    //            }
    //        }

    //        // Creating start node and pushing into priority queue.
    //        Node* startNode = new Node(m_firstSegmentPosInSteps,m_secondSegmentPosInSteps );
    //        startNode->setHScore( hypot(xStart-x,yStart-y) );
    //        nodesPriorityQueue.push( startNode );
    //        openNodes[abs(m_firstSegmentPosInSteps)][abs(m_secondSegmentPosInSteps)] = true;

    //        while ( !nodesPriorityQueue.empty() )
    //        {
    //            // Get node with the best score.
    //            Node* node = nodesPriorityQueue.top();
    //            nodesPriorityQueue.pop();

    //            openNodes[abs(node->getStepsForFristSegment())][abs(node->getStepsForSecondSegment())] = 0;
    //            closedNodes[abs(node->getStepsForFristSegment())][abs(node->getStepsForSecondSegment())] = true;

    //            // Test case.
    //            double firstSegmentAngle = computeFirstSegmentAngleByStepsPosition(node->getStepsForFristSegment());
    //            double secondSegmentAngle = computeSecondSegmentAngleByStepsPosition(node->getStepsForSecondSegment());
    //            double xNode = computeXCoordinate(firstSegmentAngle,secondSegmentAngle);
    //            double yNode = computeYCoordinate(firstSegmentAngle,secondSegmentAngle);

    //            if ( hypot(xNode-x,yNode-y) < e ) // Then searching completed successfully.
    //            {
    ////                qDebug() << node->getStepsForFristSegment();
    ////                qDebug() << node->getStepsForSecondSegment();

    //                break;
    //            }

    //            // If test case failure, we have to contiune searching.
    //            // To do this we have to generate nodes in all possible directions
    //            // and push them into priority queue, to find the most efficient one.

    //            // CCW step for first segment
    //            if ( (m_firstSegmentHomingOrientation == CCW && node->getStepsForFristSegment() > 0) ||
    //                 (m_firstSegmentHomingOrientation == CW && abs(node->getStepsForFristSegment()) < m_motor1maxSteps) )
    //            {
    //                if ( !closedNodes[abs(node->getStepsForFristSegment()-1)][abs(node->getStepsForSecondSegment())] &&
    //                     openNodes[abs(node->getStepsForFristSegment()-1)][abs(node->getStepsForSecondSegment())] == 0)
    //                {
    //                    Node* ccwForFirstSegment = new Node(node->getStepsForFristSegment()-1,node->getStepsForSecondSegment());

    //                    double new_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(ccwForFirstSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(ccwForFirstSegment->getStepsForSecondSegment()));
    //                    double new_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(ccwForFirstSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(ccwForFirstSegment->getStepsForSecondSegment()));

    //                    ccwForFirstSegment->setGScore(std::max(ccwForFirstSegment->getStepsForFristSegment()/1000.0,
    //                                                           ccwForFirstSegment->getStepsForSecondSegment()/1000.0)); // Time from start node
    //                    ccwForFirstSegment->setHScore(hypot(new_x-x,new_y-y)/1000.0); // Time to goal

    //                    nodesPriorityQueue.push(ccwForFirstSegment);
    //                    openNodes[abs(ccwForFirstSegment->getStepsForFristSegment())][abs(ccwForFirstSegment->getStepsForSecondSegment())] = ccwForFirstSegment->getScore();
    //                }
    //            }


    //            // CW step for first segment
    //            if ( (m_firstSegmentHomingOrientation == CCW && abs(node->getStepsForFristSegment()) < m_motor1maxSteps) ||
    //                 (m_firstSegmentHomingOrientation == CW && node->getStepsForFristSegment() < 0) )
    //            {
    //                if ( !closedNodes[abs(node->getStepsForFristSegment()+1)][abs(node->getStepsForSecondSegment())] &&
    //                     openNodes[abs(node->getStepsForFristSegment()+1)][abs(node->getStepsForSecondSegment())] == 0)
    //                {
    //                    Node* cwForFirstSegment = new Node(node->getStepsForFristSegment()+1,node->getStepsForSecondSegment());

    //                    double new_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(cwForFirstSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(cwForFirstSegment->getStepsForSecondSegment()));
    //                    double new_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(cwForFirstSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(cwForFirstSegment->getStepsForSecondSegment()));

    //                    cwForFirstSegment->setGScore(std::max(cwForFirstSegment->getStepsForFristSegment()/1000.0,
    //                                                          cwForFirstSegment->getStepsForSecondSegment()/1000.0)); // Time from start node
    //                    cwForFirstSegment->setHScore(hypot(new_x-x,new_y-y)/1000.0); // Time to goal


    //                    nodesPriorityQueue.push(cwForFirstSegment);
    //                    openNodes[abs(cwForFirstSegment->getStepsForFristSegment())][abs(cwForFirstSegment->getStepsForSecondSegment())] = cwForFirstSegment->getScore();
    //                }
    //            }

    //            // CCW step for second segment
    //            if ( (m_secondSegmentHomingOrientation == CCW && node->getStepsForSecondSegment() > 0) ||
    //                 (m_secondSegmentHomingOrientation == CW && abs(node->getStepsForSecondSegment()) < m_motor2maxSteps) )
    //            {
    //                if ( !closedNodes[abs(node->getStepsForFristSegment())][abs(node->getStepsForSecondSegment()-1)] &&
    //                     openNodes[abs(node->getStepsForFristSegment())][abs(node->getStepsForSecondSegment())-1] == 0)
    //                {
    //                    Node* ccwForSecondSegment = new Node(node->getStepsForFristSegment(),node->getStepsForSecondSegment()-1);

    //                    double new_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(ccwForSecondSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(ccwForSecondSegment->getStepsForSecondSegment()));
    //                    double new_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(ccwForSecondSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(ccwForSecondSegment->getStepsForSecondSegment()));

    //                    ccwForSecondSegment->setGScore(std::max(ccwForSecondSegment->getStepsForFristSegment()/1000.0,
    //                                                          ccwForSecondSegment->getStepsForSecondSegment()/1000.0)); // Time from start node
    //                    ccwForSecondSegment->setHScore(hypot(new_x-x,new_y-y)/1000.0); // Time to goal


    //                    nodesPriorityQueue.push(ccwForSecondSegment);
    //                    openNodes[abs(ccwForSecondSegment->getStepsForFristSegment())][abs(ccwForSecondSegment->getStepsForSecondSegment())] = ccwForSecondSegment->getScore();
    //                }
    //            }

    //            // CW step for second segment
    //            if ( (m_secondSegmentHomingOrientation == CCW && abs(node->getStepsForSecondSegment()) < m_motor2maxSteps) ||
    //                 (m_secondSegmentHomingOrientation == CW && node->getStepsForSecondSegment() < 0) )
    //            {
    //                if ( !closedNodes[abs(node->getStepsForFristSegment())][abs(node->getStepsForSecondSegment()+1)] &&
    //                     openNodes[abs(node->getStepsForFristSegment())][abs(node->getStepsForSecondSegment()+1)] == 0 )
    //                {
    //                    Node* cwForSecondSegment = new Node(node->getStepsForFristSegment(),node->getStepsForSecondSegment()+1);

    //                    double new_x = computeXCoordinate(computeFirstSegmentAngleByStepsPosition(cwForSecondSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(cwForSecondSegment->getStepsForSecondSegment()));
    //                    double new_y = computeYCoordinate(computeFirstSegmentAngleByStepsPosition(cwForSecondSegment->getStepsForFristSegment()),
    //                                                      computeSecondSegmentAngleByStepsPosition(cwForSecondSegment->getStepsForSecondSegment()));

    //                    cwForSecondSegment->setGScore(std::max(cwForSecondSegment->getStepsForFristSegment()/1000.0,
    //                                                          cwForSecondSegment->getStepsForSecondSegment()/1000.0)); // Time from start node
    //                    cwForSecondSegment->setHScore(hypot(new_x-x,new_y-y)/1000.0); // Time to goal

    //                    nodesPriorityQueue.push(cwForSecondSegment);
    //                    openNodes[abs(cwForSecondSegment->getStepsForFristSegment())][abs(cwForSecondSegment->getStepsForSecondSegment())] = cwForSecondSegment->getScore();
    //                }
    //            }

    //            delete node;
    //        }

    qDebug() << "Average time: " << timer.elapsed()/100;

    return QStringList();
}















