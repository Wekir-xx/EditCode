#include "header/TextEditObserver.h"

#include <QStringList>
#include <QMetaType>

TextEditObserver::TextEditObserver(QTextEdit* edit_code, QTextEdit* number_code)
    : _edit_code{edit_code},
    _number_code{number_code}
{
    _number_code->insertPlainText("   " + QString::number(1));

    _edit_scroll_bar = _edit_code->verticalScrollBar();
    _number_scroll_bar = _number_code->verticalScrollBar();

    qRegisterMetaType<QTextCursor>("QTextCursor");

    connect(_edit_scroll_bar, &QScrollBar::valueChanged, this, [this]() {
        QMetaObject::invokeMethod(this, "ChangeNumberScroll", Qt::QueuedConnection);
    });
    connect(_number_scroll_bar, &QScrollBar::valueChanged, this, [this]() {
        QMetaObject::invokeMethod(this, "ChangeCodeScroll", Qt::QueuedConnection);
    });
    connect(_edit_code, &QTextEdit::textChanged, this, [this]() {
        QMetaObject::invokeMethod(this, "ChangeCountString", Qt::QueuedConnection);
    });
}

TextEditObserver::~TextEditObserver()
{
    _edit_scroll_bar->deleteLater();
    _number_scroll_bar->deleteLater();
}

void TextEditObserver::ChangeNumberScroll()
{
    QMetaObject::invokeMethod(_number_scroll_bar, "setValue", Qt::QueuedConnection,
                              Q_ARG(int, _edit_scroll_bar->value()));
}

void TextEditObserver::ChangeCodeScroll()
{
    QMetaObject::invokeMethod(_edit_scroll_bar, "setValue", Qt::QueuedConnection,
                              Q_ARG(int, _number_scroll_bar->value()));
}

void TextEditObserver::ChangeCountString()
{
    size_t count_string = _edit_code->toPlainText().count("\n") + 1;

    while (_count_string < count_string)
        QMetaObject::invokeMethod(_number_code, "insertPlainText", Qt::QueuedConnection,
                                  Q_ARG(QString, "\n   " + QString::number(++_count_string)));

    while (_count_string > count_string)
    {
        QTextCursor cursor = _number_code->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        cursor.deleteChar();

        --_count_string;
    }
}
