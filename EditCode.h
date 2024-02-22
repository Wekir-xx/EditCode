#ifndef EDITCODE_H
#define EDITCODE_H

#include <TextEditObserver.h>
#include <HighLighter.h>

#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QApplication>
#include <QLineEdit>
#include <QList>
#include <QtWidgets/QListWidget>
#include <QPushButton>
#include <QDialog>
#include <QSplitter>

class EditCode : public QMainWindow
{
    Q_OBJECT

public:
    EditCode(QWidget *parent = nullptr);
    ~EditCode();

private slots:
    void Run();
    void NewFile();
    void OpenFile();
    void SaveFile();
    void NewFile_Create(QString name_new_file);
    void NewFile_Cancel();
    void SelectFile(QListWidgetItem *item);
    void Remove(QString remove_file);
    void All_Remove();
    void setCordsApp(bool tr) const;

private:
    QString GetNameFile(size_t number_file) const;
    void PushFile(QString pathfile);
    void CMD(QStringList& list);
    void CreateMenuBar();
    void CreateMainApp();

private:
    HighLighter* _lighter;
    TextEditObserver* _observer;
    QSplitter* _splitter;
    QLabel* _name_file;
    QTextEdit* _edit_code;
    QTextEdit* _number_code;
    QListWidget* _list_widget;
    QDialog* _dialog_newfile;

private:
    QListWidgetItem* remove_file{};
    QPushButton* _But_Remove;
    QPushButton* _But_All_Remove;

private:
    QList<QString> _path_file;
    QList<QString> _file_code;
    size_t _index;
};

#endif // EDITCODE_H
