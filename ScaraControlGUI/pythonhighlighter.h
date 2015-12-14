#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class PythonHighlighter : public QSyntaxHighlighter
{
public:
    PythonHighlighter(QTextDocument* parent=0);

protected:
    void highlightBlock(QString const& text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat singleQuotedFormat;
    QTextCharFormat multiQuotedFormat;
    QTextCharFormat numericLiteralsFormat;

    bool matchMultiline(const QString &text, const QRegExp &delimiter, const int inState, const QTextCharFormat &style);

signals:

public slots:
};

#endif // PYTHONHIGHLIGHTER_H
