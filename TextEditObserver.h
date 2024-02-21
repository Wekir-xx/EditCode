#ifndef TEXTEDITOBSERVER_H
#define TEXTEDITOBSERVER_H

#include <QObject>
#include <QTextEdit>
#include <QScrollBar>

class TextEditObserver : public QObject
{
    Q_OBJECT
public:
    TextEditObserver(QTextEdit* edit_code, QTextEdit* number_code);
    ~TextEditObserver();

private slots:
    void ChangeScroll();
    void ChangeCountString();

private:
    QTextEdit* _edit_code;
    QTextEdit* _number_code;
    size_t _count_string{1};
    QScrollBar* _edit_scroll_bar;
    QScrollBar* _number_scroll_bar;
};

#endif // TEXTEDITOBSERVER_H
