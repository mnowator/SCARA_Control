#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>


class TextEdit : public QTextEdit
{
public:
    QString path;

    TextEdit(QWidget* parent);
    TextEdit(QString const& name,QWidget* parent);

signals:

public slots:
};

#endif // TEXTEDIT_H
