#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include <QScreen>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("EditCode");
    this->setWindowIcon(QIcon(":/images/icon.png"));

    CMD(QStringList() << "/C" << "mkdir" << "C:\\EditCode");
    CMD(QStringList() << "/C" << "mkdir" << "C:\\EditCode\\file");

    CreateMenuBar();
    CreateMainApp();
}

MainWindow::~MainWindow()
{
    _name_file->deleteLater();
    _edit_code->deleteLater();
    _list_widget->deleteLater();
}

void MainWindow::Run()
{
    if(_path_file.size() == 0)
        NewFile();
    if(_path_file.size() == 0)
        return;

    this->setEnabled(false);

    size_t time_index = _index;
    for(qint8 i {}; i < _path_file.size(); ++i)
    {
        _index = i;
        SaveFile();
    }
    _index = time_index;

    QString main_file = GetNameFile(0);
    size_t i{};
    for(auto rev_iter = main_file.begin(); *rev_iter != '.'; ++rev_iter, ++i);

    main_file = main_file.left(i);
    main_file = "C:/EditCode/file/" + main_file + ".exe";

    QStringList compilation{};

    compilation << "/C" << "g++";

    for(auto& iter_path : _path_file)
        compilation<< iter_path;

    CMD(compilation << "-o" << main_file);
    CMD(QStringList() << "/C" << "start" << main_file);

    this->setEnabled(true);
}

void MainWindow::NewFile()
{
    this->setEnabled(false);

    _dialog_newfile = new QDialog(this);
    _dialog_newfile->setFixedSize(360, 125);
    _dialog_newfile->setWindowIcon(QIcon(":/images/icon.png"));
    _dialog_newfile->setWindowTitle("New File - EditCode");
    _dialog_newfile->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QLabel* text_name = new QLabel(_dialog_newfile);
    QLineEdit* edit_name = new QLineEdit(_dialog_newfile);
    QPushButton* but_create = new QPushButton(_dialog_newfile);
    QPushButton* but_cancel = new QPushButton(_dialog_newfile);

    text_name->setText("Name File:");
    edit_name->setText("Application");
    but_create->setText("Create");
    but_cancel->setText("Cancel");

    text_name->setGeometry(10, 15, 70, 20);
    edit_name->setGeometry(80, 15, 150, 20);
    but_create->setGeometry(30, 60, 75, 30);
    but_cancel->setGeometry(155, 60, 75, 30);

    connect(_dialog_newfile, &QDialog::finished, this, [this](){
        if(!this->isEnabled())
            this->setEnabled(true);
    });
    connect(but_create, &QPushButton::pressed, this, [this, edit_name](){
        NewFile_Create(edit_name->displayText());
    });
    connect(but_cancel, &QPushButton::pressed, this, &MainWindow::NewFile_Cancel);

    _dialog_newfile->show();
}

void MainWindow::OpenFile()
{
    this->setEnabled(false);

    QString path_new_file = QFileDialog::getOpenFileName(this, tr("Open file"), "C:/", tr("cpp file (*.cpp);;h file (*.h);;hpp file (*.hpp)"));

    if(path_new_file.size() == 0)
    {
        this->setEnabled(true);
        return;
    }

    QFile file(path_new_file);
    if (file.open(QIODevice::ReadOnly))
    {
        if(_path_file.size() != 0)
        {
            auto iter_code = _file_code.begin();
            for(size_t i{}; i < _index; ++i, ++iter_code);

            *iter_code = _edit_code->toPlainText();
        }

        PushFile(path_new_file);
        _name_file->setText (GetNameFile(_index));
        _edit_code->setText (QString(file.readAll()));

        file.close();

        auto new_iter_code = _file_code.begin();
        for(size_t i{}; i < _index; ++i, ++new_iter_code);

        *new_iter_code = _edit_code->toPlainText();
    }
    else
        _name_file->setText("Not open file");

    this->setEnabled(true);
}

void MainWindow::SaveFile()
{
    if(_path_file.size() == 0)
    {
        NewFile();
        return;
    }

    auto iter_path = _path_file.begin();
    for(size_t i{}; i < _index; ++i, ++iter_path);

    QFile file(*iter_path);
    if (file.open(QIODevice::WriteOnly))
    {
        auto iter_code = _file_code.begin();
        for(size_t i{}; i < _index; ++i, ++iter_code);

        if(GetNameFile(_index) == _name_file->text())
            *iter_code = _edit_code->toPlainText();

        file.write((*iter_code).toUtf8());
        file.close();
    }
    else
        _name_file->setText("Not save file");
}

void MainWindow::NewFile_Create(QString name_new_file)
{
    if(name_new_file.size() == 0)
        name_new_file = "Application";

    QFile file("C:/EditCode/file/" + name_new_file + ".cpp");
    if (file.open(QIODevice::WriteOnly))
    {
        file.close();

        if (_path_file.size() != 0)
        {
            auto iter_code = _file_code.begin();
            for(size_t i{}; i < _index; ++i, ++iter_code);
            *iter_code = _edit_code->toPlainText();

            _edit_code->clear();
        }

        _name_file->setText (name_new_file + ".cpp");
        PushFile("C:/EditCode/file/" + name_new_file + ".cpp");
    }
    else
        _name_file->setText("Not create file");

    if(_path_file.size() == 1)
        SaveFile();

    _dialog_newfile->deleteLater();
    this->setEnabled(true);
}

void MainWindow::NewFile_Cancel()
{
    _dialog_newfile->deleteLater();
    this->setEnabled(true);
}

void MainWindow::SelectFile(QListWidgetItem *item)
{
    auto iter_path = _path_file.begin();
    auto iter_code = _file_code.begin();
    for(size_t i{}; iter_path != _path_file.end(); ++i, ++iter_path, ++iter_code)
    {
        if(GetNameFile(i) == item->text())
        {
            if(_path_file.size() != 1)
            {
                auto new_iter_code = _file_code.begin();
                for(size_t i{}; i < _index; ++i, ++new_iter_code);

                *new_iter_code = _edit_code->toPlainText();
            }

            _index = i;
            _edit_code->setText(*iter_code);
            _name_file->setText(item->text());

            return;
        }
    }
}

void MainWindow::Remove(QString remove_file)
{
    auto iter_path = _path_file.begin();
    auto iter_code = _file_code.begin();
    size_t i{};
    for(; remove_file != GetNameFile(i); ++i, ++iter_path, ++iter_code);

    if(i == _index || _path_file.size() == 1)
    {
        _name_file->setText("");
        _edit_code->setText("");
    }

    _path_file.erase(iter_path);
    _file_code.erase(iter_code);
    _index = 0;
}

void MainWindow::All_Remove()
{
    while(_path_file.size() != 0)
    {
        _path_file.pop_front();
        _file_code.pop_front();
    }

    _name_file->setText("");
    _edit_code->setText("");
    _list_widget->clear();
}

void MainWindow::setCordsApp(bool tr)
{
    if(tr)
    {
        _name_file->setGeometry(20, 25, 700, 35);
        _edit_code->setGeometry(10, 55, qApp->primaryScreen()->availableGeometry().width() - 350,
                                qApp->primaryScreen()->availableGeometry().height() - 94);
        _list_widget->setGeometry(qApp->primaryScreen()->availableGeometry().width() - 330, 55, 320,
                                  qApp->primaryScreen()->availableGeometry().height() - 500);
        _But_Remove->setGeometry(qApp->primaryScreen()->availableGeometry().width() - 330,
                                qApp->primaryScreen()->availableGeometry().height() - 435, 320, 40);
        _But_All_Remove->setGeometry(qApp->primaryScreen()->availableGeometry().width() - 330,
                                    qApp->primaryScreen()->availableGeometry().height() - 385, 320, 40);
    }
    else
    {
        _name_file->setGeometry(350, 25, 700, 35);
        _edit_code->setGeometry(340, 55, qApp->primaryScreen()->availableGeometry().width() - 354,
                                qApp->primaryScreen()->availableGeometry().height() - 94);
        _list_widget->setGeometry(10, 55, 320, qApp->primaryScreen()->availableGeometry().height() - 500);
        _But_Remove->setGeometry(10, qApp->primaryScreen()->availableGeometry().height() - 435, 320, 40);
        _But_All_Remove->setGeometry(10, qApp->primaryScreen()->availableGeometry().height() - 385, 320, 40);
    }
}


QString MainWindow::GetNameFile(size_t number_file)
{
    auto iter_path = _path_file.begin();
    for(size_t i{}; i < number_file; ++i, ++iter_path);

    size_t i{};
    for(auto rev_iter = iter_path->rbegin(); *rev_iter != '/'; ++rev_iter, ++i);

    return iter_path->right(i);
}

void MainWindow::PushFile(QString path_new_file)
{
    auto iter_path = _path_file.begin();
    for(qint8 i{}; i < _path_file.size(); ++i, ++iter_path)
    {
        if(*iter_path == path_new_file)
        {
            _index = i;
            return;
        }
    }

    _path_file.push_back(path_new_file);
    _file_code.push_back("");
    _index = _path_file.size() - 1;

    _list_widget->addItem(new QListWidgetItem(QIcon(":/images/list_icon.png"), GetNameFile(_index)));
}

void MainWindow::CMD(QStringList& list)
{
    QProcess myProcess(this);
    myProcess.start("cmd", list);
    myProcess.waitForFinished();
}

void MainWindow::CreateMenuBar()
{
    menuBar()->setStyleSheet("background-color: #D3D3D3");

    QAction *new_file = new QAction(QIcon(":/images/new.png"), "&New File", this);
    QAction *open_file = new QAction(QIcon(":/images/open.png"), "&Open File", this);
    QAction *save_file = new QAction(QIcon(":/images/save.png"), "&Save File", this);
    QAction *quit = new QAction("&Quit", this);
    QAction *run = new QAction("&Run (CTRL+F5)", this);
    QAction* left_form = new QAction("&Left Form", this);
    QAction* right_form = new QAction("&Right Form", this);

    left_form->setCheckable(true);
    right_form->setCheckable(true);

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);
    left_form->setActionGroup(group);
    right_form->setActionGroup(group);

    left_form->setChecked(true);

    new_file->setShortcut(tr("CTRL+N"));
    open_file->setShortcut(tr("CTRL+O"));
    save_file->setShortcut(tr("CTRL+S"));
    run->setShortcut(tr("CTRL+F5"));
    quit->setShortcut(tr("CTRL+Q"));
    left_form->setShortcut(tr("ALT+0"));
    right_form->setShortcut(tr("ALT+SHIFT+0"));

    connect(quit, &QAction::triggered, this, &QApplication::quit);
    connect(new_file, &QAction::triggered, this, &MainWindow::NewFile);
    connect(open_file, &QAction::triggered, this, &MainWindow::OpenFile);
    connect(save_file, &QAction::triggered, this, &MainWindow::SaveFile);
    connect(run, &QAction::triggered, this, &MainWindow::Run);
    connect(left_form, &QAction::triggered, this, [this](){
        this->setEnabled(false);

        setCordsApp(true);

        this->setEnabled(true);
    });
    connect(right_form, &QAction::triggered, this, [this](){
        this->setEnabled(false);

        setCordsApp(false);

        this->setEnabled(true);
    });

    QMenu *file_bar = menuBar()->addMenu("File");
    menuBar()->addAction(run);
    QMenu *view_bar = menuBar()->addMenu("View");

    file_bar->addAction(new_file);
    file_bar->addAction(open_file);
    file_bar->addAction(save_file);
    file_bar->addSeparator();
    file_bar->addAction(quit);

    view_bar->addAction(left_form);
    view_bar->addAction(right_form);

    file_bar->setStyleSheet("QMenu::item:selected { background-color: #A9A9A9;"
                            "color: black; }");
    view_bar->setStyleSheet("QMenu::item:selected { background-color: #A9A9A9;"
                            "color: black; }");
}

void MainWindow::CreateMainApp()
{
    _name_file = new QLabel(this);
    _edit_code = new QTextEdit(this);
    _list_widget = new QListWidget(this);

    _name_file->setStyleSheet("font-size:11pt; font-weight: 550;");
    _edit_code->setStyleSheet("font-size:10pt; font-weight: 450;");
    _list_widget->setStyleSheet("font-size:10pt; font-weight: 550;");

    QTextOption textOption = _edit_code->document()->defaultTextOption();
    textOption.setTabStopDistance(20);
    _edit_code->document()->setDefaultTextOption(textOption);

    _But_Remove = new QPushButton(this);
    _But_All_Remove = new QPushButton(this);

    _But_Remove->setText("Remove File");
    _But_All_Remove->setText("Remove All File");

    _But_Remove->setStyleSheet("font-size:10pt; font-weight: 500;");
    _But_All_Remove->setStyleSheet("font-size:10pt; font-weight: 500;");

    _But_Remove->setShortcut(QKeySequence(Qt::Key_Delete));
    _But_All_Remove->setShortcut(QKeySequence(Qt::Key_Delete));

    connect(_list_widget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item){
        if(_path_file.size() != 0)
            SelectFile(item);
    });
    connect(_list_widget, &QListWidget::itemClicked, [&](QListWidgetItem *item){
        if(remove_file == item)
            remove_file = nullptr;
        else
            remove_file = item;
    });
    connect(_But_Remove, &QPushButton::pressed, this, [&](){
        if(remove_file != nullptr)
        {
            _list_widget->setEnabled(false);

            Remove(remove_file->text());

            QListWidgetItem* item = _list_widget->takeItem(_list_widget->row(remove_file));
            delete item;
            remove_file = nullptr;

            _list_widget->setEnabled(true);
        }   
    });
    connect(_But_All_Remove, &QPushButton::pressed, this, [&](){
        _list_widget->setEnabled(false);

        All_Remove();
        remove_file = nullptr;

        _list_widget->setEnabled(true);
    });

    setCordsApp(true);
}
