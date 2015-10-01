#ifndef SCARAROBOT_H
#define SCARAROBOT_H

#include "abstractcommunicationlayer.h"
#include "scaralogic.h"

class Q_DECL_EXPORT ScaraRobot
{
private:

    QString m_projectName;

    AbstractCommunicationLayer* m_communicationLayer;
    ScaraLogic* m_scaraLogic;

public:
    ScaraRobot();

    class Exception
    {
    protected:
        QString m_errorMsg;

    public:
        Exception();
        Exception(QString qStr) :
            m_errorMsg(qStr)
        {

        }

        virtual void setMsg(QString qStr)
        {
            m_errorMsg = qStr;
        }

        virtual QString const& what() const
        {
            return m_errorMsg;
        }
    };
};

#endif // SCARAROBOT_H
