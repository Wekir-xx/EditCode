#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class HighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    HighLighter(QTextDocument *parent = nullptr);
    void ChangeTheme(bool _color_style);

private:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> HighHightingRules;

private:
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    QTextCharFormat MultiLineCommentFormat;

private:
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
};

#endif // HIGHLIGHTER_H
