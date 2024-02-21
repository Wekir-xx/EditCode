#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QApplication>
#include <QLineEdit>
#include <QList>
#include <QtWidgets/QListWidget>
#include <QPushButton>
#include <QDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    void setCordsApp(bool tr);

private:
    QString GetNameFile(size_t number_file);
    void PushFile(QString pathfile);
    void CMD(QStringList& list);
    void CreateMenuBar();
    void CreateMainApp();

private:
    QLabel* _name_file;
    QTextEdit* _edit_code;
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

#endif // MAINWINDOW_H
