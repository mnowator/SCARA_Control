#ifndef ABSTRACTLAYER_H
#define ABSTRACTLAYER_H

#include <QObject>

class AbstractLayer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractLayer(QObject *parent = 0);

signals:

public slots:
};

#endif // ABSTRACTLAYER_H
