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
    edit_code->setStyleSheet("font-size:10pt; font-weight: 450;");

    QTextOption textOption = edit_code->document()->defaultTextOption();
    textOption.setTabStopDistance(20);
    edit_code->document()->setDefaultTextOption(textOption);
}

MainWindow::~MainWindow()
{
    name_file->deleteLater();
    edit_code->deleteLater();
}

void MainWindow::Run()
{
    if(path_file.size() == 0) NewFile();
    if(path_file.size() == 0) return;
    this->setEnabled(false);

    auto itc = file_code.begin();
    for(size_t i{}; i < index; ++i, ++itc );
    *itc = edit_code->toPlainText();

    size_t for_index = index;
    for(qint8 i {}; i < path_file.size(); ++i)
    {
        index = i;
        SaveFile();
    }
    index = for_index;

    QString name_cpp {Name(0)};
    size_t i = 0;
    for(auto rit = name_cpp.begin(); *rit != '.'; ++rit, ++i);
    name_cpp = name_cpp.left(i);
    name_cpp = "C:/EditCode/file/" + name_cpp + ".exe";

    QProcess myProcess(this);
    QStringList compilation;

    compilation << "/C" << "g++";
    for(auto itp {path_file.begin()}; itp != path_file.end(); ++itp)
    {
        compilation<< *itp;
    }
    compilation<< "-o" << name_cpp;

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
    if (path_file.size() != 0) SaveFile();

    mw_newfile = new QMainWindow(this);
    mw_newfile->setFixedSize(360, 125);
    mw_newfile->setWindowIcon(QIcon(":/images/icon.png"));
    mw_newfile->setWindowTitle("New File - EditCode");

    QLabel* text_name = new QLabel(mw_newfile);
    QLineEdit* edit_name = new QLineEdit(mw_newfile);
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

    connect(create, &QPushButton::pressed, this, [this, edit_name]() {
        NewFile_Create(edit_name->displayText());
    });
    connect(cancel, &QPushButton::pressed, this, &MainWindow::NewFile_Cancel);
}

void MainWindow::OpenFile()
{
    if(path_file.size() != 0) SaveFile();

    QString pathfile {QFileDialog::getOpenFileName(this, tr("Open file"), "C:/", tr("cpp file (*.cpp);;h file (*.h);;hpp file (*.hpp)"))};
    if(pathfile.size() == 0) return;

    QFile file(pathfile);
    if (!file.open(QIODevice::ReadOnly))
    {
        name_file->setText("Not open file");
    }
    else
    {
        auto itc = file_code.begin();
        for(size_t i{}; i < index; ++i, ++itc );
        *itc = edit_code->toPlainText();

        push_file(pathfile);

        name_file->setText (Name(index));
        edit_code->setText (QString(file.readAll()));
        file.close();
    }
}

void MainWindow::SaveFile()
{
    if(path_file.size() == 0)
    {
        NewFile();
        return;
    }

    auto itp = path_file.begin();
    for(size_t i{}; i < index; ++i, ++itp );

    QFile file(*itp);
    if (!file.open(QIODevice::WriteOnly))
    {
        name_file->setText("Not save file");
    }
    else
    {
        auto itc = file_code.begin();
        for(size_t i{}; i < index; ++i, ++itc );

        file.write(itc->toUtf8());
        file.close();
    }
}

void MainWindow::NewFile_Create(QString name_new_file)
{
    if(name_new_file.size() == 0) name_new_file = "Application";
    name_file->setText (name_new_file + ".cpp");

    QFile file("C:/EditCode/file/" + name_new_file + ".cpp");
    if (file.open(QIODevice::WriteOnly))
    {
        if (path_file.size() != 0)
        {
            auto itc = file_code.begin();
            for(size_t i{}; i < index; ++i, ++itc );
            *itc = edit_code->toPlainText();

            edit_code->clear();
        }

        push_file("C:/EditCode/file/" + name_new_file + ".cpp");
        file.close();
    }
    else
    {
        name_file->setText("Not create file");
    }

    mw_newfile->deleteLater();
    this->setEnabled(true);

    if(path_file.size() == 1) SaveFile();
}

void MainWindow::NewFile_Cancel()
{
    mw_newfile->deleteLater();
    this->setEnabled(true);
}

void MainWindow::Remove()
{
    QString remove_file;

    auto itp = path_file.begin();
    auto itc = file_code.begin();
    for(size_t i{}; remove_file != Name(i); ++i, ++itp, ++itc );

    path_file.erase(itp);
    file_code.erase(itc);
}

void MainWindow::All_Remove()
{
    qint8 size = path_file.size();
    for(qint8 i{}; i < size; ++i)
    {
        path_file.pop_front();
        file_code.pop_front();
    }
    index = 0;
}

QString MainWindow::Name(size_t number_file)
{
    auto itp = path_file.begin();
    for(size_t i{}; i < number_file; ++i, ++itp );

    size_t i{};
    for(auto rit = itp->rbegin(); *rit != '/'; ++rit, ++i);

    return itp->right(i);
}

void MainWindow::push_file(QString pathfile)
{
    auto itp = path_file.begin();
    for(qint8 i{}; i < path_file.size(); ++i, ++itp )
    {
        if(*itp == pathfile)
        {
            index = i;
            return;
        }
    }
    path_file.push_back(pathfile);
    file_code.push_back("");
    index = path_file.size() - 1;
}
