#include "codeeditor.h"

#include <QtWidgets>

#include "styles.h"

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(undoAvailable(bool)),this, SLOT(undoAvailable(bool)));
    connect(this, SIGNAL(redoAvailable(bool)),this, SLOT(redoAvailable(bool)));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setStyleSheet(currentCodeEditorTheme);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPixelSize(12);

    setFont(font);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequested(QPoint)));
}

CodeEditor::CodeEditor(const QString &name, QWidget *parent)
    : QPlainTextEdit(name,parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

bool CodeEditor::isUndoAvailable() const
{
    return undoIsAvailable;
}

bool CodeEditor::isRedoAvailable() const
{
    return redoIsAvailable;
}

void CodeEditor::turnOnPythonHighlighting()
{
    highlighter = new PythonHighlighter(this->document());
}

void CodeEditor::turnOffPythonHighlighting()
{
    if ( highlighter )
        delete highlighter;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        if ( event->delta() > 0 )
        {
            QFont f = font();
            const int newSize = f.pixelSize() + 1;
            if (newSize <= 0)
                return;

            f.setPixelSize(newSize);
            setFont(f);
        }
        else if ( event->delta() < 0)
        {
            QFont f = font();
            const int newSize = f.pixelSize() - 1;
            if (newSize <= 0)
                return;

            f.setPixelSize(newSize);
            setFont(f);
        }

        return;
    }

    QPlainTextEdit::wheelEvent(event);
}

void CodeEditor::undoAvailable(bool available)
{
    undoIsAvailable = available;
}

void CodeEditor::redoAvailable(bool available)
{
    redoIsAvailable = available;
}

void CodeEditor::customContextMenuRequested(const QPoint &pos )
{
    QMenu menu(this);

    QAction* undo = new QAction(QIcon(":/new/icons/lc_undo.png"),tr("Undo"),this);
    undo->setShortcut(QKeySequence::Undo);
    connect(undo,SIGNAL(triggered(bool)),this,SLOT(undo()));

    if ( !undoIsAvailable )
        undo->setEnabled(false);

    menu.addAction(undo);


    QAction* redo = new QAction(QIcon(":/new/icons/lc_redo.png"),tr("Redo"),this);
    redo->setShortcut(QKeySequence::Redo);
    connect(redo,SIGNAL(triggered(bool)),this,SLOT(redo()));

    if ( !redoIsAvailable )
        redo->setEnabled(false);

    menu.addAction(redo);


    menu.addSeparator();


    QAction* cut = new QAction(QIcon(":/new/icons/lc_cut.png"),tr("Cut"),this);
    cut->setShortcut(QKeySequence::Cut);
    connect(cut,SIGNAL(triggered(bool)),this,SLOT(cut()));

    if ( document()->isEmpty() )
        cut->setEnabled(false);

    menu.addAction(cut);


    QAction* copy = new QAction(QIcon(":/new/icons/lc_copy.png"),tr("Copy"),this);
    copy->setShortcut(QKeySequence::Copy);
    connect(copy,SIGNAL(triggered(bool)),this,SLOT(copy()));

    if ( document()->isEmpty() )
        copy->setEnabled(false);

    menu.addAction(copy);


    QAction* paste = new QAction(QIcon(":/new/icons/lc_paste.png"),tr("Paste"),this);
    paste->setShortcut(QKeySequence::Undo);
    connect(paste,SIGNAL(triggered(bool)),this,SLOT(paste()));

    if ( !canPaste() )
        paste->setEnabled(false);

    menu.addAction(paste);


    QAction* _delete = new QAction(tr("Delete"),this);
    _delete->setShortcut(QKeySequence::Delete);
    connect(_delete,SIGNAL(triggered(bool)),this,SLOT(deleteSelection()));
    menu.addAction(_delete);



    menu.addSeparator();

    QAction* selectAll = new QAction(tr("Select All"),this);
    selectAll->setShortcut(QKeySequence::SelectAll);
    connect(selectAll,SIGNAL(triggered(bool)),this,SLOT(selectAll()));
    menu.addAction(selectAll);

    menu.setStyleSheet(currentMenuBarTheme);

    menu.exec(this->mapToGlobal(pos));
}

void CodeEditor::deleteSelection()
{
    foreach( QTextEdit::ExtraSelection selection, this->extraSelections() )
        selection.cursor.removeSelectedText();
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(darkHighlightLineTheme);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(darkLineNumberingRectTheme));
    //painter.fillRect(QRect(event->rect().right()-1,0,1,event->rect().height()),QColor(230,230,230));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QPen(darkLineNumberingPenTheme));
            painter.drawText(0, top, lineNumberArea->width()-4, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
