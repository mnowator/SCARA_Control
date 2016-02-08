#include "doublevalidator.h"

DoubleValidator::DoubleValidator(double bottom, double top, int decimals, QObject *parent)
    : QDoubleValidator(bottom,top,decimals,parent)
{

}

#include <QDebug>

void DoubleValidator::fixup(QString &input) const
{
    if ( input.isEmpty() )
        input = QString::number(0);

    if ( input.toDouble() < bottom() )
        input = QString::number(bottom());
    else if ( input.toDouble() > top() )
        input = QString::number(top());
}

