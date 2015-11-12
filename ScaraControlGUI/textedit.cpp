#include "textedit.h"

TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent)
{

}

TextEdit::TextEdit(const QString &name, QWidget *parent)
    : QTextEdit(name,parent)
{

}

