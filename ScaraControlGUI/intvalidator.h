#ifndef INTVALIDATOR_H
#define INTVALIDATOR_H

#include <QIntValidator>

class IntValidator : public QIntValidator
{
public:
    IntValidator(int bottom, int top, QObject *parent=0);

protected:
    void fixup(QString& input) const;

signals:

public slots:
};

#endif // INTVALIDATOR_H
