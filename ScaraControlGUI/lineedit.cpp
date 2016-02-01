#include "lineedit.h"
#include "styles.h"

#include <QtWidgets>

LineEdit::LineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuRequested(QPoint)));
}

void LineEdit::contextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);

    QAction* undo = new QAction(QIcon(":/new/icons/lc_undo.png"),tr("Undo"),this);
    undo->setShortcut(QKeySequence::Undo);
    connect(undo,SIGNAL(triggered(bool)),this,SLOT(undo()));

    if ( !isUndoAvailable() )
        undo->setEnabled(false);

    menu.addAction(undo);

    QAction* redo = new QAction(QIcon(":/new/icons/lc_redo.png"),tr("Redo"),this);
    redo->setShortcut(QKeySequence::Redo);
    connect(redo,SIGNAL(triggered(bool)),this,SLOT(redo()));

    if ( !isRedoAvailable() )
        redo->setEnabled(false);

    menu.addAction(redo);

    menu.addSeparator();

    QAction* cut = new QAction(QIcon(":/new/icons/lc_cut.png"),tr("Cut"),this);
    cut->setShortcut(QKeySequence::Cut);
    connect(cut,SIGNAL(triggered(bool)),this,SLOT(cut()));

    if ( !hasSelectedText() )
        cut->setEnabled(false);

    menu.addAction(cut);

    QAction* copy = new QAction(QIcon(":/new/icons/lc_copy.png"),tr("Copy"),this);
    copy->setShortcut(QKeySequence::Copy);
    connect(copy,SIGNAL(triggered(bool)),this,SLOT(copy()));

    if ( !hasSelectedText() )
        copy->setEnabled(false);

    menu.addAction(copy);

    QAction* paste = new QAction(QIcon(":/new/icons/lc_paste.png"),tr("Paste"),this);
    paste->setShortcut(QKeySequence::Undo);
    connect(paste,SIGNAL(triggered(bool)),this,SLOT(paste()));

    if ( !isEnabled() )
        paste->setEnabled(false);

    menu.addAction(paste);

    menu.addSeparator();

    QAction* selectAll = new QAction(tr("Select All"),this);
    selectAll->setShortcut(QKeySequence::SelectAll);
    connect(selectAll,SIGNAL(triggered(bool)),this,SLOT(selectAll()));

    menu.addAction(selectAll);

    menu.setStyleSheet(currentMenuBarTheme);
    menu.exec(mapToGlobal(pos));
}

