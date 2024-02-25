#include "HighLighter.h"

HighLighter::HighLighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    TypeFormat.setForeground(Qt::darkBlue);

    const QString keywordTypePatterns[] =
        {
            QStringLiteral("\\bchar\\b"), QStringLiteral("\\bint\\b"), QStringLiteral("\\bdouble\\b"),
            QStringLiteral("\\bshort\\b"), QStringLiteral("\\blong\\b"), QStringLiteral("\\bvoid\\b"),
            QStringLiteral("\\bconst\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstatic\\b"),
            QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bsigned\\b"),
            QStringLiteral("\\bvirtual\\b"), QStringLiteral("\\bpublic\\b"), QStringLiteral("\\bprotected\\b"),
            QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bstd\\b"), QStringLiteral("\\boverride\\b"),
            QStringLiteral("\\bsize_t\\b")
        };


    for (const QString &pattern : keywordTypePatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = TypeFormat;
        HighHightingRules.append(rule);
    }

    BoolFormat.setForeground(Qt::darkRed);
    BoolFormat.setFontWeight(QFont::Bold);
    BoolFormat.setFontItalic(true);

    const QString keywordBoolPatterns[] =
        {
            QStringLiteral("\\bfalse\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\bbool\\b"),
        };

    for (const QString &pattern : keywordBoolPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = BoolFormat;
        HighHightingRules.append(rule);
    }

    SpecialFormat.setForeground(Qt::darkBlue);
    SpecialFormat.setFontWeight(QFont::Bold);

    const QString keywordSpecialPatterns[] =
        {
            QStringLiteral("\\bwhile\\b"), QStringLiteral("\\bfor\\b"), QStringLiteral("\\bnamespace\\b"),
            QStringLiteral("\\binline\\b"), QStringLiteral("\\busing\\b"), QStringLiteral("\\bclass\\b"),
            QStringLiteral("\\bunion\\b"), QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"),
            QStringLiteral("\\btypename\\b"), QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\bsignals\\b"), QStringLiteral("\\benum\\b")
        };

    for (const QString &pattern : keywordSpecialPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = SpecialFormat;
        HighHightingRules.append(rule);
    }

    OtherSpecialFormat.setForeground(Qt::blue);
    OtherSpecialFormat.setFontWeight(QFont::Bold);

    const QString keywordOtherSpecialPatterns[] =
        {
            QStringLiteral("\\bfriend\\b"), QStringLiteral("\\boperator\\b"), QStringLiteral("\\bexplicit\\b"),
            QStringLiteral("\\breturn\\b")
        };

    for (const QString &pattern : keywordOtherSpecialPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = OtherSpecialFormat;
        HighHightingRules.append(rule);
    }

    PreProcessFormat.setForeground(Qt::darkGray);
    PreProcessFormat.setFontWeight(QFont::Bold);

    const QString keywordPreProcessPatterns[] =
        {
            QStringLiteral("\\#\\binclude\\b"), QStringLiteral("\\#\\bdefine\\b"), QStringLiteral("\\#\\belse\\b"),
            QStringLiteral("\\#\\berror\\b"), QStringLiteral("\\#\\bifdef\\b"), QStringLiteral("\\#\\bifndef\\b"),
            QStringLiteral("\\#\\bendif\\b"), QStringLiteral("\\#\\bif\\b"), QStringLiteral("\\#\\belif\\b"),
            QStringLiteral("\\#\\bpragma once\\b")
        };

    for (const QString &pattern : keywordPreProcessPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = PreProcessFormat;
        HighHightingRules.append(rule);
    }

    SemiliconFormat.setForeground(Qt::black);
    SemiliconFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\;"));
    rule.format = SemiliconFormat;
    HighHightingRules.append(rule);

    FunctionFormat.setForeground(Qt::darkYellow);
    FunctionFormat.setFontItalic(true);
    rule.pattern = QRegularExpression(QStringLiteral("\\b(?!for\\b|while\\b)[A-Za-z0-9_]+(?=\\()"));
    rule.format = FunctionFormat;
    HighHightingRules.append(rule);

    QtFormat.setFontWeight(QFont::Bold);
    QtFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = QtFormat;
    HighHightingRules.append(rule);

    QuotationFormat.setForeground(Qt::darkMagenta);
    QuotationFormat.setFontItalic(true);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = QuotationFormat;
    HighHightingRules.append(rule);

    TriangleBracketsFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("<.*>"));
    rule.format = TriangleBracketsFormat;
    HighHightingRules.append(rule);

    SingleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = SingleLineCommentFormat;
    HighHightingRules.append(rule);

    MultiLineCommentFormat.setForeground(Qt::darkGreen);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void HighLighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(HighHightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);

        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);
    int startIndex{};

    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength{};

        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }

        setFormat(startIndex, commentLength, MultiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
