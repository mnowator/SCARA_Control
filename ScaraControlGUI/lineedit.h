#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>


class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit(QWidget* parent=0);

signals:

public slots:
    void contextMenuRequested(const QPoint &pos);
};

#endif // LINEEDIT_H
