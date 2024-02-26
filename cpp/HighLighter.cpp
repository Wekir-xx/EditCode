#include "header/HighLighter.h"

HighLighter::HighLighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    BoolFormat.setForeground(Qt::darkRed);
    BoolFormat.setFontWeight(QFont::Bold);

    OtherSpecialFormat.setForeground(Qt::blue);

    PreProcessFormat.setForeground(Qt::darkGray);
    PreProcessFormat.setFontWeight(QFont::Bold);

    SemiliconFormat.setFontWeight(QFont::Bold);

    FunctionFormat.setFontItalic(true);

    QuotationFormat.setForeground(Qt::darkRed);
    QuotationFormat.setFontItalic(true);

    TriangleBracketsFormat.setForeground(Qt::darkMagenta);

    SingleLineCommentFormat.setForeground(Qt::darkGreen);

    MultiLineCommentFormat.setForeground(Qt::darkGreen);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void HighLighter::ChangeTheme(bool _color_style)
{
    HighlightingRule rule;
    HighHightingRules.clear();

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

    const QString keywordBoolPatterns[] =
        {
            QStringLiteral("\\bfalse\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\bbool\\b"),
        };

    const QString keywordSpecialPatterns[] =
        {
            QStringLiteral("\\bwhile\\b"), QStringLiteral("\\bfor\\b"), QStringLiteral("\\bnamespace\\b"),
            QStringLiteral("\\binline\\b"), QStringLiteral("\\busing\\b"), QStringLiteral("\\bclass\\b"),
            QStringLiteral("\\bunion\\b"), QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"),
            QStringLiteral("\\btypename\\b"), QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\bsignals\\b"), QStringLiteral("\\benum\\b")
        };

    const QString keywordOtherSpecialPatterns[] =
        {
            QStringLiteral("\\bfriend\\b"), QStringLiteral("\\boperator\\b"), QStringLiteral("\\bexplicit\\b"),
            QStringLiteral("\\breturn\\b")
        };

    const QString keywordPreProcessPatterns[] =
        {
            QStringLiteral("\\#\\binclude\\b"), QStringLiteral("\\#\\bdefine\\b"), QStringLiteral("\\#\\belse\\b"),
            QStringLiteral("\\#\\berror\\b"), QStringLiteral("\\#\\bifdef\\b"), QStringLiteral("\\#\\bifndef\\b"),
            QStringLiteral("\\#\\bendif\\b"), QStringLiteral("\\#\\bif\\b"), QStringLiteral("\\#\\belif\\b"),
            QStringLiteral("\\#\\bpragma once\\b")
        };

    const QString keywordSemilicon = QStringLiteral("\\;");

    const QString keywordFunction = QStringLiteral("\\b(?!for\\b|while\\b)[A-Za-z0-9_]+(?=\\()");

    const QString keywordQt = QStringLiteral("\\bQ[A-Za-z]+\\b");

    const QString keywordQuotation = QStringLiteral("\".*\"");

    const QString keywordTriangle = QStringLiteral("<.*>");

    const QString keywordSingleLineComment = QStringLiteral("//[^\n]*");

    if(_color_style)
    {
        TypeFormat.setForeground(Qt::darkBlue);

        SpecialFormat.setForeground(Qt::darkBlue);
        SpecialFormat.setFontWeight(QFont::Bold);

        OtherSpecialFormat.setFontWeight(QFont::Bold);

        SemiliconFormat.setForeground(Qt::black);

        FunctionFormat.setForeground(Qt::darkYellow);

        QtFormat.setForeground(Qt::darkBlue);
        QtFormat.setFontWeight(QFont::Bold);
    }
    else
    {
        TypeFormat.setForeground(Qt::blue);

        SpecialFormat.setForeground(Qt::green);
        SpecialFormat.setFontWeight(QFont::Normal);

        OtherSpecialFormat.setFontWeight(QFont::Normal);

        SemiliconFormat.setForeground(Qt::white);

        FunctionFormat.setForeground(Qt::yellow);

        QtFormat.setForeground(Qt::darkBlue);
        QtFormat.setFontWeight(QFont::Bold);
    }

    for (const QString &pattern : keywordTypePatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = TypeFormat;
        HighHightingRules.append(rule);
    }

    for (const QString &pattern : keywordBoolPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = BoolFormat;
        HighHightingRules.append(rule);
    }

    for (const QString &pattern : keywordSpecialPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = SpecialFormat;
        HighHightingRules.append(rule);
    }

    for (const QString &pattern : keywordOtherSpecialPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = OtherSpecialFormat;
        HighHightingRules.append(rule);
    }

    for (const QString &pattern : keywordPreProcessPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = PreProcessFormat;
        HighHightingRules.append(rule);
    }

    rule.pattern = QRegularExpression(keywordSemilicon);
    rule.format = SemiliconFormat;
    HighHightingRules.append(rule);

    rule.pattern = QRegularExpression(keywordFunction);
    rule.format = FunctionFormat;
    HighHightingRules.append(rule);

    rule.pattern = QRegularExpression(keywordQt);
    rule.format = QtFormat;
    HighHightingRules.append(rule);

    rule.pattern = QRegularExpression(keywordQuotation);
    rule.format = QuotationFormat;
    HighHightingRules.append(rule);

    rule.pattern = QRegularExpression(keywordTriangle);
    rule.format = TriangleBracketsFormat;
    HighHightingRules.append(rule);

    rule.pattern = QRegularExpression(keywordSingleLineComment);
    rule.format = SingleLineCommentFormat;
    HighHightingRules.append(rule);

    rehighlight();
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
