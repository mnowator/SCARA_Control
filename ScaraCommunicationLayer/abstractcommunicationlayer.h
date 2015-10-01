#ifndef ABSTRACTCOMMUNICATIONLAYER_H
#define ABSTRACTCOMMUNICATIONLAYER_H

#include <QObject>

class Q_DECL_EXPORT AbstractCommunicationLayer : public QObject
{
public:
    explicit AbstractCommunicationLayer(QObject *parent = 0);

signals:

public slots:
};

#endif // ABSTRACTCOMMUNICATIONLAYER_H
