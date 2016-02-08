#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "pythonhighlighter.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    QString path;

    CodeEditor(QWidget* parent = 0);
    CodeEditor(QString const& name,QWidget* parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    bool isUndoAvailable() const;
    bool isRedoAvailable() const;

    void turnOnPythonHighlighting();
    void turnOffPythonHighlighting();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void undoAvailable(bool available);
    void redoAvailable(bool available);
    void customContextMenuRequested(const QPoint &pos);
    void deleteSelection();
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;
    PythonHighlighter* highlighter;

    bool undoIsAvailable;
    bool redoIsAvailable;

    unsigned numOfIndents = 0;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};



#endif
