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

private:
    QString NameFile(size_t number_file);
    void PushFile(QString pathfile);
    void CreateDirectory();
    void CreateMenuBar();
    void CreateMainApp();

private:
    QListWidget* _list_widget;
    QList<QString> _path_file;
    QList<QString> _file_code;
    size_t _index;
    QLabel* _name_file;
    QTextEdit* _edit_code;
    QMainWindow* _MW_newfile;
};

#endif // MAINWINDOW_H
