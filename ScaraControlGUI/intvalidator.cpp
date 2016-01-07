#include "intvalidator.h"

IntValidator::IntValidator(int bottom, int top, QObject* parent)
    : QIntValidator(bottom,top,parent)
{

}

void IntValidator::fixup(QString &input) const
{
    if( input.toInt() < bottom() )
        input = QString::number(bottom());
    else if( input.toInt() > top() )
        input = QString::number(top());
}

