#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QApplication>
#include <QLineEdit>

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
    void NewFile_Create();
    void NewFile_Cancel();

private:
    QString path_file;
    QLabel* name_file;
    QTextEdit* edit_code;
    QMainWindow* mw_newfile;
    QLineEdit* edit_name;
};
#endif // MAINWINDOW_H
