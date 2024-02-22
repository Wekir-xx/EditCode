#include "TextEditObserver.h"

#include <QStringList>

TextEditObserver::TextEditObserver(QTextEdit *edit_code, QTextEdit *number_code)
    : _edit_code{edit_code},
    _number_code{number_code}
{
    _edit_scroll_bar = _edit_code->verticalScrollBar();
    _number_scroll_bar = _number_code->verticalScrollBar();

    connect(_edit_scroll_bar, &QScrollBar::valueChanged, this, &TextEditObserver::ChangeScroll);
    connect(_edit_code, &QTextEdit::textChanged, this, &TextEditObserver::ChangeCountString);
}

TextEditObserver::~TextEditObserver()
{
    _edit_scroll_bar->deleteLater();
    _number_scroll_bar->deleteLater();
}

void TextEditObserver::ChangeScroll()
{
    _number_scroll_bar->setValue(_edit_scroll_bar->value());
}

void TextEditObserver::ChangeCountString()
{
    size_t count_string = _edit_code->toPlainText().count("\n") + 1;

    if(_count_string < count_string)
    {
        while (_count_string < count_string)
            _number_code->insertHtml("<div align=\"right\">" +
                                     QString::number(++_count_string) + "</div><br>");

        _number_scroll_bar->setValue(_edit_scroll_bar->value());
    }
    else if(_count_string > count_string)
    {
        while (_count_string > count_string)
        {
            QTextCursor cursor(_number_code->document());
            cursor.movePosition(QTextCursor::End);
            cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

            if (!cursor.selectedText().isEmpty())
            {
                cursor.movePosition(QTextCursor::StartOfLine);
                cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
                cursor.removeSelectedText();
                cursor.deleteChar();
            }
            --_count_string;
        }
        _number_scroll_bar->setValue(_edit_scroll_bar->value());
    }

}
