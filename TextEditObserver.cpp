#include "TextEditObserver.h"

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
    if(count_string != _count_string)
    {
        _count_string = count_string;
        _number_code->clear();

        for (size_t i = 1; i <= _count_string; ++i)
            _number_code->insertHtml("<div align=\"right\">"
                                     + QString::number(i) + "</div><br>");

        _number_scroll_bar->setValue(_edit_scroll_bar->value());
    }
}
