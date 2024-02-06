#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(this->frameGeometry().width(), this->frameGeometry().height());
    this->setWindowTitle("EditCode");
    this->setWindowIcon(QIcon(":/images/icon.png"));
    menuBar()->setStyleSheet ("background-color: #D3D3D3");

    QMenu *file_bar = menuBar()->addMenu("File");
    QAction *new_file = new QAction(QIcon(":/images/new.png"), "&New File", this);
    QAction *open_file = new QAction(QIcon(":/images/open.png"), "&Open File", this);
    QAction *save_file = new QAction(QIcon(":/images/save.png"), "&Save File", this);
    QAction *quit = new QAction("&Quit", this);
    QAction *run = new QAction("&Run (CTRL+F5)", this);

    new_file->setShortcut (tr("CTRL+N"));
    open_file->setShortcut (tr("CTRL+O"));
    save_file->setShortcut (tr("CTRL+S"));
    run->setShortcut (tr("CTRL+F5"));
    quit->setShortcut (tr("CTRL+Q"));

    connect (quit, &QAction::triggered, this, &QApplication::quit);
    connect (new_file, &QAction::triggered, this, &MainWindow::NewFile);
    connect (open_file, &QAction::triggered, this, &MainWindow::OpenFile);
    connect (save_file, &QAction::triggered, this, &MainWindow::SaveFile);
    connect (run, &QAction::triggered, this, &MainWindow::Run);

    file_bar->addAction(new_file);
    file_bar->addAction(open_file);
    file_bar->addAction(save_file);
    file_bar->addSeparator ();
    file_bar->addAction(quit);
    menuBar()->addAction(run);

    QLabel *name_file = new QLabel(this);
    QTextEdit *edit_code = new QTextEdit(this);
    name_file->setGeometry (20, 25, 500, 35);
    edit_code->setGeometry (10, 55, qApp->primaryScreen()->availableGeometry().width() - 20, qApp->primaryScreen()->availableGeometry().height() - 90);

    //name_file->setText ("<html><head/><body><p><span style=\" font-size:10pt; font-weight:700;\">main.cpp</span></p></body></html>");
}

MainWindow::~MainWindow()
{

}

void MainWindow::Run()
{

}

void MainWindow::NewFile()
{

}

void MainWindow::OpenFile()
{

}

void MainWindow::SaveFile()
{

}
