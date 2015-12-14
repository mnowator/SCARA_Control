#include "pythonhighlighter.h"

PythonHighlighter::PythonHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor("darkorange"));
    //keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << "and"    << "assert" << "break"  << "class"  << "continue" << "def"      <<
                       "del"    << "elif"   << "else"   << "except" << "exec"     << "finally"  <<
                       "for"    << "from"   << "global" << "if"     << "import"   << "in"       <<
                       "is"     << "lambda" << "not"    << "or"     << "pass"     << "print"    <<
                       "raise"  << "return" << "try"    << "while"  << "yield"    << "None"     <<
                       "import"   << "from";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp("\\b"+pattern+"\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    QStringList functionPatterns;
    functionFormat.setForeground(QColor("plum"));
    //functionFormat.setFontWeight(QFont::Bold);

    functionPatterns << "True" << "False" << "int" << "float" << "list" << "dict" << "open"
                      << "abs" << "all" << "basestring" << "bin" << "bool" << "bytearray"
                      << "callable" << "chr" << "classmethod" << "cmp" << "compile" << "complex"
                      << "delattr" << "dir" << "divmod" << "enumerate" << "eval" << "execfile"
                      << "file" << "filter" << "format" << "frozenset" << "getattr" << "globals"
                      << "hasattr" << "hash" << "help" << "hex" << "id" << "input" << "isinstance"
                      << "iter" << "len" << "locals" << "long" << "map" << "max" << "memoryview"
                      << "min" << "next" << "object" << "oct" << "org" << "pow"
                      << "property" << "range" << "raw_input" << "reduce" << "reload" << "repr"
                      << "reversed" << "round" << "set" << "setattr" << "slice" << "sorted"
                      << "staticmethod" << "str" << "sum" << "super" << "tuple" << "type"
                      << "unichr" << "unicode" << "vars" << "xrange" << "zip" << "self";

    foreach (const QString &pattern, functionPatterns)
    {
        rule.pattern = QRegExp("\\b"+pattern+"\\b");
        rule.format = functionFormat;
        highlightingRules.append(rule);
    }

    QStringList numericLiteralPatterns;
    numericLiteralsFormat.setForeground(QColor("slateblue"));
    //numericLiteralsFormat.setFontWeight(QFont::Bold);

    numericLiteralPatterns << "\\b[+-]?[0-9]+[lL]?\\b"
                           << "\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b"
                           << "\\b[+-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b";

    foreach (const QString &pattern, numericLiteralPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = numericLiteralsFormat;
        highlightingRules.append(rule);
    }

    commentFormat.setForeground(QColor("indianred"));
    //commentFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("#[^\\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    singleQuotedFormat.setForeground(QColor("darkseagreen"));
    //singleQuotedFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("'[^'\\\\]*(\\\\.[^'\\\\]*)*'");
    rule.format = singleQuotedFormat;
    highlightingRules.append(rule);

    multiQuotedFormat.setForeground(QColor("darkseagreen"));
    //multiQuotedFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"");
    rule.format = multiQuotedFormat;
    highlightingRules.append(rule);
}


void PythonHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }


    setCurrentBlockState(0);

    // Do multi-line strings
    bool isInMultilne = matchMultiline(text, QRegExp("'''"), 1, singleQuotedFormat);

    if (!isInMultilne)
      isInMultilne = matchMultiline(text, QRegExp("\"\"\""), 2, singleQuotedFormat);
}

bool PythonHighlighter::matchMultiline(const QString &text, const QRegExp &delimiter, const int inState, const QTextCharFormat &style)
{
    int start = -1;
    int add = -1;
    int end = -1;
    int length = 0;

    // If inside triple-single quotes, start at 0
    if (previousBlockState() == inState)
    {
        start = 0;
        add = 0;
    }
    // Otherwise, look for the delimiter on this line
    else
    {
        start = delimiter.indexIn(text);
        // Move past this match
        add = delimiter.matchedLength();
    }

    // As long as there's a delimiter match on this line...
    while (start >= 0)
    {
        // Look for the ending delimiter
        end = delimiter.indexIn(text, start + add);
        // Ending delimiter on this line?
        if (end >= add)
        {
            length = end - start + add + delimiter.matchedLength();
            setCurrentBlockState(0);
        }
        // No; multi-line string
        else
        {
            setCurrentBlockState(inState);
            length = text.length() - start + add;
        }
        // Apply formatting and look for next
        setFormat(start, length, style);
        start = delimiter.indexIn(text, start + length);
    }
    // Return True if still inside a multi-line string, False otherwise
    if (currentBlockState() == inState)
        return true;
    else
        return false;
}
