#ifndef DOUBLEVALIDATOR_H
#define DOUBLEVALIDATOR_H

#include <QDoubleValidator>

class DoubleValidator : public QDoubleValidator
{
public:
    DoubleValidator(double bottom, double top, int decimals, QObject *parent=0);

protected:
    void fixup(QString& input) const;

signals:

public slots:
};

#endif // DOUBLEVALIDATOR_H
