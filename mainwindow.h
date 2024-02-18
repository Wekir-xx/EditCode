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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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
    QListWidget* list_widget;
    QList<QString> path_file;
    QList<QString> file_code;
    size_t index{};
    QLabel* name_file;
    QTextEdit* edit_code;
    QMainWindow* mw_newfile;

    QString Name(size_t number_file);
    void push_file(QString pathfile);
};
#endif // MAINWINDOW_H
