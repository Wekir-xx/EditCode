#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle ("EditCode");
    this->setWindowIcon (QIcon(":/images/icon.png"));
    this->showMaximized ();

    QMenu *file_bar = menuBar()->addMenu("File");
    QAction *new_file = new QAction(QIcon(":/images/icon.png"), "&New File", this);
    QAction *open_file = new QAction(QIcon(":/images/icon.png"), "&Open File", this);
    QAction *save_file = new QAction(QIcon(":/images/icon.png"), "&Save File", this);
    QAction *run = new QAction("&Run", this);
    QAction *quit = new QAction("&Quit", this);

    quit->setShortcut (tr("CTRL+Q"));
    new_file->setShortcut (tr("CTRL+N"));
    open_file->setShortcut (tr("CTRL+O"));
    save_file->setShortcut (tr("CTRL+F5"));

    connect (quit, &QAction::triggered, this, &QApplication::quit);

    file_bar->addAction(new_file);
    file_bar->addAction(open_file);
    file_bar->addSeparator ();
    file_bar->addAction(save_file);
    menuBar()->addAction(run);
    menuBar()->addAction(quit);

    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Run()
{
    ui->label->setText ("i work");
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
