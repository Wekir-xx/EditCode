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
    list_widget = new QListWidget(this);

    name_file->setGeometry(20, 25, 700, 35);
    edit_code->setGeometry(10, 55, qApp->primaryScreen()->availableGeometry().width() - 350,
                           qApp->primaryScreen()->availableGeometry().height() - 94);
    list_widget->setGeometry(qApp->primaryScreen()->availableGeometry().width() - 330, 55, 320,
                             qApp->primaryScreen()->availableGeometry().height() - 500);

    name_file->setStyleSheet("font-size:11pt; font-weight: 550;");
    edit_code->setStyleSheet("font-size:10pt; font-weight: 450;");
    list_widget->setStyleSheet("font-size:10pt; font-weight: 550;");

    QTextOption textOption = edit_code->document()->defaultTextOption();
    textOption.setTabStopDistance(20);
    edit_code->document()->setDefaultTextOption(textOption);

    QPushButton* But_Remove = new QPushButton(this);
    QPushButton* But_All_Remove = new QPushButton(this);

    But_Remove->setText("Remove File");
    But_All_Remove->setText("Remove All File");

    But_Remove->setGeometry(qApp->primaryScreen()->availableGeometry().width() - 330,
                            qApp->primaryScreen()->availableGeometry().height() - 435, 320, 40);
    But_All_Remove->setGeometry(qApp->primaryScreen()->availableGeometry().width() - 330,
                            qApp->primaryScreen()->availableGeometry().height() - 385, 320, 40);

    But_Remove->setStyleSheet("font-size:10pt; font-weight: 500;");
    But_All_Remove->setStyleSheet("font-size:10pt; font-weight: 500;");

    But_Remove->setShortcut(QKeySequence(Qt::Key_Delete));
    But_All_Remove->setShortcut(QKeySequence(Qt::Key_Delete));

    QListWidgetItem* remove_file{};

    connect(list_widget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item){
        if(path_file.size() != 0)
            SelectFile(item);
    });
    connect(list_widget, &QListWidget::itemClicked, [&](QListWidgetItem *item){
        if(remove_file == item)
            remove_file = nullptr;
        else
            remove_file = item;
    });
    connect(But_Remove, &QPushButton::pressed, this, [&](){
        list_widget->setEnabled(false);

        if(remove_file != nullptr)
        {
            Remove(remove_file->text());

            QListWidgetItem* item = list_widget->takeItem(list_widget->row(remove_file));
            delete item;
        }
        remove_file = nullptr;

        list_widget->setEnabled(true);
    });
    connect(But_All_Remove, &QPushButton::pressed, this, [&](){
        list_widget->setEnabled(false);

        All_Remove();
        remove_file = nullptr;

        list_widget->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
    name_file->deleteLater();
    edit_code->deleteLater();
}

void MainWindow::Run()
{
    if(path_file.size() == 0)
        NewFile();
    if(path_file.size() == 0)
        return;

    this->setEnabled(false);

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
        compilation<< *itp;

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
    if (path_file.size() != 0)
        SaveFile();

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

    connect(create, &QPushButton::pressed, this, [this, edit_name](){
        NewFile_Create(edit_name->displayText());
    });
    connect(cancel, &QPushButton::pressed, this, &MainWindow::NewFile_Cancel);
}

void MainWindow::OpenFile()
{
    this->setEnabled(false);

    if(path_file.size() != 0)
        SaveFile();

    QString pathfile {QFileDialog::getOpenFileName(this, tr("Open file"), "C:/", tr("cpp file (*.cpp);;h file (*.h);;hpp file (*.hpp)"))};

    if(pathfile.size() == 0)
        return;

    QFile file(pathfile);
    if (file.open(QIODevice::ReadOnly))
    {
        if(path_file.size() != 0)
        {
            auto itc = file_code.begin();
            for(size_t i{}; i < index; ++i, ++itc);
            *itc = edit_code->toPlainText();
        }

        push_file(pathfile);
        name_file->setText (Name(index));
        edit_code->setText (QString(file.readAll()));

        file.close();

        auto itc_2 = file_code.begin();
        for(size_t i{}; i < index; ++i, ++itc_2);

        *itc_2 = edit_code->toPlainText();
    }
    else
        name_file->setText("Not open file");

    this->setEnabled(true);
}

void MainWindow::SaveFile()
{
    if(path_file.size() == 0)
    {
        NewFile();
        return;
    }

    auto itp = path_file.begin();
    for(size_t i{}; i < index; ++i, ++itp);

    QFile file(*itp);
    if (file.open(QIODevice::WriteOnly))
    {
        auto itc = file_code.begin();
        for(size_t i{}; i < index; ++i, ++itc);

        if(Name(index) == name_file->text())
            *itc = edit_code->toPlainText();

        file.write((*itc).toUtf8());
        file.close();
    }
    else
        name_file->setText("Not save file");
}

void MainWindow::NewFile_Create(QString name_new_file)
{
    if(name_new_file.size() == 0)
        name_new_file = "Application";

    QFile file("C:/EditCode/file/" + name_new_file + ".cpp");
    if (file.open(QIODevice::WriteOnly))
    {
        file.close();

        if (path_file.size() != 0)
        {
            auto itc = file_code.begin();
            for(size_t i{}; i < index; ++i, ++itc);
            *itc = edit_code->toPlainText();

            edit_code->clear();
        }

        name_file->setText (name_new_file + ".cpp");
        push_file("C:/EditCode/file/" + name_new_file + ".cpp");
    }
    else
        name_file->setText("Not create file");

    if(path_file.size() == 1)
        SaveFile();

    mw_newfile->deleteLater();
    this->setEnabled(true);
}

void MainWindow::NewFile_Cancel()
{
    mw_newfile->deleteLater();
    this->setEnabled(true);
}

void MainWindow::SelectFile(QListWidgetItem *item)
{
    auto itp = path_file.begin();
    auto itc = file_code.begin();
    for(size_t i{}; itp != path_file.end(); ++i, ++itp, ++itc)
    {
        if(Name(i) == item->text())
        {
            if(path_file.size() != 1)
            {
                auto itc_2 = file_code.begin();
                for(size_t i{}; i < index; ++i, ++itc_2);
                *itc_2 = edit_code->toPlainText();
            }

            index = i;
            edit_code->setText(*itc);
            name_file->setText(item->text());

            return;
        }
    }
}

void MainWindow::Remove(QString remove_file)
{
    auto itp = path_file.begin();
    auto itc = file_code.begin();
    size_t i{};
    for(; remove_file != Name(i); ++i, ++itp, ++itc);

    if(i == index || path_file.size() == 1)
    {
        name_file->setText("");
        edit_code->setText("");
    }

    path_file.erase(itp);
    file_code.erase(itc);
    index = 0;
}

void MainWindow::All_Remove()
{
    qint8 size = path_file.size();
    for(qint8 i{}; i < size; ++i)
    {
        path_file.pop_front();
        file_code.pop_front();
    }

    name_file->setText("");
    edit_code->setText("");
    list_widget->clear();
}

QString MainWindow::Name(size_t number_file)
{
    auto itp = path_file.begin();
    for(size_t i{}; i < number_file; ++i, ++itp);

    size_t i{};
    for(auto rit = itp->rbegin(); *rit != '/'; ++rit, ++i);

    return itp->right(i);
}

void MainWindow::push_file(QString pathfile)
{
    auto itp = path_file.begin();
    for(qint8 i{}; i < path_file.size(); ++i, ++itp)
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

    list_widget->addItem(new QListWidgetItem(QIcon(":/images/list_icon.png"), Name(index)));
}
