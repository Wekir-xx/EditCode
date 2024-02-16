#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include <QScreen>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QProcess myProcess(this);
    QStringList compilation;

    compilation << "/C" << "mkdir" << "C:\\EditCode";
    myProcess.start("cmd", compilation);
    myProcess.waitForFinished();

    compilation.clear();

    compilation << "/C" << "mkdir" << "C:\\EditCode\\file";
    myProcess.start("cmd", compilation);
    myProcess.waitForFinished();

    this->setWindowTitle("EditCode");
    this->setWindowIcon(QIcon(":/images/icon.png"));
    menuBar()->setStyleSheet("background-color: #D3D3D3");

    QMenu *file_bar = menuBar()->addMenu("File");
    QAction *new_file = new QAction(QIcon(":/images/new.png"), "&New File", this);
    QAction *open_file = new QAction(QIcon(":/images/open.png"), "&Open File", this);
    QAction *save_file = new QAction(QIcon(":/images/save.png"), "&Save File", this);
    QAction *quit = new QAction("&Quit", this);
    QAction *run = new QAction("&Run (CTRL+F5)", this);

    new_file->setShortcut(tr("CTRL+N"));
    open_file->setShortcut(tr("CTRL+O"));
    save_file->setShortcut(tr("CTRL+S"));
    run->setShortcut(tr("CTRL+F5"));
    quit->setShortcut(tr("CTRL+Q"));

    connect(quit, &QAction::triggered, this, &QApplication::quit);
    connect(new_file, &QAction::triggered, this, &MainWindow::NewFile);
    connect(open_file, &QAction::triggered, this, &MainWindow::OpenFile);
    connect(save_file, &QAction::triggered, this, &MainWindow::SaveFile);
    connect(run, &QAction::triggered, this, &MainWindow::Run);

    file_bar->addAction(new_file);
    file_bar->addAction(open_file);
    file_bar->addAction(save_file);
    file_bar->addSeparator();
    file_bar->addAction(quit);
    menuBar()->addAction(run);

    name_file = new QLabel(this);
    edit_code = new QTextEdit(this);
    name_file->setGeometry(20, 25, 700, 35);
    edit_code->setGeometry(10, 55, qApp->primaryScreen()->availableGeometry().width() - 20, qApp->primaryScreen()->availableGeometry().height() - 94);

    name_file->setStyleSheet("font-size:10pt; font-weight: 550;");
    edit_code->setStyleSheet("font-size:9pt; font-weight: 350;");
}

MainWindow::~MainWindow()
{

}

void MainWindow::Run()
{
    SaveFile();

    if(path_file.size() < 1) return;
    this->setEnabled(false);

    QString name_cpp = Name();
    qint8 i = 0;
    for(auto rit = name_cpp.begin(); *rit != '.'; rit++, i++);
    name_cpp = name_cpp.left(i);
    name_cpp = "C:/EditCode/file/" + name_cpp + ".exe";

    QProcess myProcess(this);
    QStringList compilation;

    compilation << "/C" << "g++" << path_file << "-o" << name_cpp;
    myProcess.start("cmd", compilation);
    myProcess.waitForFinished();

    compilation.clear();

    compilation << "/C" << "start" << name_cpp;
    myProcess.start("cmd", compilation);
    myProcess.waitForFinished();

    this->setEnabled(true);
}

void MainWindow::NewFile()
{
    if (path_file.size() > 0) SaveFile();

    mw_newfile = new QMainWindow(this);
    mw_newfile->setFixedSize(360, 125);
    mw_newfile->setWindowIcon(QIcon(":/images/icon.png"));
    mw_newfile->setWindowTitle("New File - EditCode");

    QLabel* text_name = new QLabel(mw_newfile);
    edit_name = new QLineEdit(mw_newfile);
    QPushButton* create = new QPushButton(mw_newfile);
    QPushButton* cancel = new QPushButton(mw_newfile);

    text_name->setText("Name File:");
    edit_name->setText("Application");
    create->setText("Create");
    cancel->setText("Cancel");

    text_name->setGeometry(10, 15, 70, 20);
    edit_name->setGeometry(80, 15, 150, 20);
    create->setGeometry(30, 60, 75, 30);
    cancel->setGeometry(155, 60, 75, 30);

    this->setEnabled(false);
    mw_newfile->setEnabled(true);
    mw_newfile->show();

    connect(create, &QPushButton::pressed, this, &MainWindow::NewFile_Create);
    connect(cancel, &QPushButton::pressed, this, &MainWindow::NewFile_Cancel);
}

void MainWindow::OpenFile()
{
    if(path_file.size() > 0) SaveFile();

    path_file = QFileDialog::getOpenFileName(this, tr("Open file"), "C:/", tr("cpp file (*.cpp);;h file (*.h);;hpp file (*.hpp)"));
    if(path_file.size() < 1) return;

    name_file->setText (Name());

    QFile file(path_file);
    if (!file.open(QIODevice::ReadOnly))
    {
        name_file->setText("Not open file");
        return;
    }
    edit_code->setText (QString(file.readAll()));
    file.close();
}

void MainWindow::SaveFile()
{
    if(path_file.size() < 1) NewFile();

    QFile file(path_file);
    if (!file.open(QIODevice::WriteOnly))
    {
        name_file->setText("Not save file");
        return;
    }
    file.write((edit_code->toPlainText()).toUtf8());
    file.close();
}

void MainWindow::NewFile_Create()
{
    QString name_new_file = edit_name->displayText();    
    if(name_new_file.size() < 1) name_new_file = "Application";

    name_file->setText (name_new_file + ".cpp");
    if (path_file.size() > 0) edit_code->clear();

    path_file = "C:/EditCode/file/" + name_new_file + ".cpp";

    QFile file(path_file);
    if (!file.open(QIODevice::WriteOnly))
    {
        name_file->setText("Not create file");
    }
    file.close();

    mw_newfile->deleteLater();
    edit_name->deleteLater();
    this->setEnabled(true);
}

void MainWindow::NewFile_Cancel()
{
    mw_newfile->deleteLater();
    edit_name->deleteLater();
    this->setEnabled(true);
}

QString MainWindow::Name()
{
    qint8 i = 0;
    for(auto rit = path_file.rbegin(); *rit != '/'; rit++, i++);
    return path_file.right(i);
}
