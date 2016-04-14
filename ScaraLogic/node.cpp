#include "node.h"


Node::Node(int stepsForFirstSegment, int stepsForSecondSegment)
    : m_stepsForFirstSegment(stepsForFirstSegment),
      m_stepsForSecondSegment(stepsForSecondSegment)
{

}

int Node::getStepsForFristSegment() const
{
    return m_stepsForFirstSegment;
}

int Node::getStepsForSecondSegment() const
{
    return m_stepsForSecondSegment;
}

double Node::getGScore() const
{
    return m_gScore;
}

double Node::getHScore() const
{
    return m_hScore;
}

double Node::getScore() const
{
    return m_gScore + m_hScore;
}

void Node::setGScore(double score)
{
    m_gScore = score;
}

void Node::setHScore(double score)
{
    m_hScore = score;
}

bool Node::operator <(const Node &right) const
{
    return this->getScore() > right.getScore();
}
