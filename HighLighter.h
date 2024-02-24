#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class HighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    HighLighter(QTextDocument *parent = nullptr);

private:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QList<HighlightingRule> HighHightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat TypeFormat;
    QTextCharFormat BoolFormat;
    QTextCharFormat SpecialFormat;
    QTextCharFormat OtherSpecialFormat;
    QTextCharFormat PreProcessFormat;
    QTextCharFormat SemiliconFormat;
    QTextCharFormat FunctionFormat;
    QTextCharFormat QtFormat;
    QTextCharFormat QuotationFormat;
    QTextCharFormat TriangleBracketsFormat;
    QTextCharFormat SingleLineCommentFormat;
    QTextCharFormat MultiLineCommentFormat;
};

#endif // HIGHLIGHTER_H
