#ifndef NODE_H
#define NODE_H


class Node
{
private:
    int m_stepsForFirstSegment = 0;
    int m_stepsForSecondSegment = 0;

    double m_gScore = 0;
    double m_hScore = 0;

public:
    Node(int stepsForFirstSegment, int stepsForSecondSegment);

    int getStepsForFristSegment() const;
    int getStepsForSecondSegment() const;

    double getGScore() const;
    double getHScore() const;
    double getScore() const;

    void setGScore(double score);
    void setHScore(double score);

    bool operator < ( const Node& right) const;
};

#endif // NODE_H
