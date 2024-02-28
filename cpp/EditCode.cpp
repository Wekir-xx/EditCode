#include "header/EditCode.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include <QScreen>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QProcess>

EditCode::EditCode(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle("EditCode");
    this->setWindowIcon(QIcon(":/images/icon.png"));
    this->resize(1100, 700);

    CMD(QStringList() << "/C" << "mkdir" << "C:\\EditCode");
    CMD(QStringList() << "/C" << "mkdir" << "C:\\EditCode\\file");

    _VLayout = new QVBoxLayout(this);

    CreateMenuBar();
    CreateMainApp();

    _observer = new TextEditObserver(_edit_code, _number_code);
    _lighter = new HighLighter(_edit_code->document());

    setFormApp();
    setStyleApp();

    _thread_observer = new QThread();
    connect(this, SIGNAL(destroyed()), _thread_observer, SLOT(quit()));
    _observer->moveToThread(_thread_observer);
    _thread_observer->start();
}

EditCode::~EditCode()
{
    delete _observer;
    delete _lighter;
}

void EditCode::Run()
{
    if(_path_file.isEmpty())
        NewFile();
    if(_path_file.isEmpty())
        return;

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
    main_file = "C:\\EditCode\\file\\" + main_file + ".exe";

    QStringList compilation{};

    compilation << "/C" << "g++";

    for(auto& iter_path : _path_file)
        compilation<< iter_path;

    CMD(QStringList() << "/C" << "del" << "/f" << "/q" << main_file);
    CMD(compilation << "-o" << main_file);
    CMD(QStringList() << "/C" << "start" << main_file);
}

void EditCode::NewFile()
{
    this->setEnabled(false);
    _menuBar->setEnabled(false);

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
    connect(but_cancel, &QPushButton::pressed, this, &EditCode::NewFile_Cancel);

    _dialog_newfile->show();
}

void EditCode::OpenFile()
{
    QString path_new_file = QFileDialog::getOpenFileName(this, tr("Open file"), "C:/", tr("cpp file (*.cpp);;h file (*.h);;hpp file (*.hpp)"));

    if(path_new_file.isEmpty())
    {
        this->setEnabled(true);
        _menuBar->setEnabled(true);
        return;
    }

    QFile file(path_new_file);
    if (file.open(QIODevice::ReadOnly))
    {
        if(!_path_file.isEmpty())
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
}

void EditCode::SaveFile()
{
    if(_path_file.isEmpty())
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

void EditCode::NewFile_Create(QString name_new_file)
{
    if(name_new_file.isEmpty())
        name_new_file = "Application";

    QFile file("C:/EditCode/file/" + name_new_file + ".cpp");
    if (file.open(QIODevice::WriteOnly))
    {
        file.close();

        if (!_path_file.isEmpty())
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
    _menuBar->setEnabled(true);
}

void EditCode::NewFile_Cancel()
{
    _dialog_newfile->deleteLater();
    this->setEnabled(true);
    _menuBar->setEnabled(true);
}

void EditCode::SelectFile(QListWidgetItem *item)
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

void EditCode::Remove(QString remove_file)
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

void EditCode::All_Remove()
{
    while(!_path_file.isEmpty())
    {
        _path_file.pop_front();
        _file_code.pop_front();
    }

    _name_file->setText("");
    _edit_code->setText("");
    _list_widget->clear();
}

void EditCode::setFormApp()
{
    if(_layout_form)
    {
        if(!_HLayout->isEmpty())
        {
            _HLayout->removeItem(_HLayout->takeAt(0));
            _HLayout->removeItem(_HLayout->takeAt(0));
            auto item = _HLayout_name->takeAt(0);
            delete item->widget();
            delete item;

            _spacer = new QSpacerItem(_number_code->width() * 1.1, 0);
            _HLayout_name->insertSpacerItem(0, _spacer);
        }
        _HLayout->addWidget(_splitter);
        _HLayout->addLayout(_VLayout_list);
    }
    else
    {
        _HLayout->removeItem(_HLayout->takeAt(0));
        _HLayout->removeItem(_HLayout->takeAt(0));
        auto item = _HLayout_name->takeAt(0);
        delete item->widget();
        delete item;

        _HLayout->addLayout(_VLayout_list);
        _HLayout->addWidget(_splitter);

        _spacer = new QSpacerItem(_list_widget->width() + _number_code->width() * 1.2, 0);
        _HLayout_name->insertSpacerItem(0, _spacer);
    }

    _HLayout->setStretchFactor(_splitter, 5);
    _HLayout->setStretchFactor(_VLayout_list, 1);
}

void EditCode::setStyleApp()
{
    ChangeImage();
    _lighter->ChangeTheme(_color_style);

    if(_color_style)
    {
        this->setObjectName("QMainWindowWhite");
        _menuBar->setObjectName("QMenuBarWhite");
        _file_bar->setObjectName("QMenuWhite");
        _views_bar->setObjectName("QMenuWhite");
        _name_file->setObjectName("QLabelWhite");
        _number_code->setObjectName("QTextEditWhite");
        _edit_code->setObjectName("QTextEditWhite");
        _edit_code->verticalScrollBar()->setObjectName("QScrollBarWhite");
        _list_widget->setObjectName("QListWidgetWhite");
        _But_Remove->setObjectName("QPushButtonWhite");
        _But_All_Remove->setObjectName("QPushButtonWhite");
    }
    else
    {
        this->setObjectName("QWidgetBlack");
        _menuBar->setObjectName("QMenuBarBlack");
        _file_bar->setObjectName("QMenuBlack");
        _views_bar->setObjectName("QMenuBlack");
        _name_file->setObjectName("QLabelBlack");
        _number_code->setObjectName("QTextEditBlack");
        _edit_code->setObjectName("QTextEditBlack");
        _edit_code->verticalScrollBar()->setObjectName("QScrollBarBlack");
        _list_widget->setObjectName("QListWidgetBlack");
        _But_Remove->setObjectName("QPushButtonBlack");
        _But_All_Remove->setObjectName("QPushButtonBlack");
    }

    QFile file(":/styles.qss");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(QLatin1String(file.readAll()));
    file.close();
}

QString EditCode::GetNameFile(size_t number_file) const
{
    auto iter_path = _path_file.begin();
    for(size_t i{}; i < number_file; ++i, ++iter_path);

    size_t i{};
    for(auto rev_iter = iter_path->rbegin(); *rev_iter != '/'; ++rev_iter, ++i);

    return iter_path->right(i);
}

void EditCode::PushFile(QString path_new_file)
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

    _list_widget->addItem(new QListWidgetItem(QIcon(_list_icon), GetNameFile(_index)));
}

void EditCode::CMD(QStringList& list)
{   
    QProcess myProcess(this);
    myProcess.start("cmd", list);
    myProcess.waitForFinished();
}

void EditCode::ChangeImage()
{
    if(_color_style)
    {
        _list_icon = ":/images/white_theme/list_icon.png";
        new_file->setIcon(QIcon(":/images/white_theme/new.png"));
        open_file->setIcon(QIcon(":/images/white_theme/open.png"));
        save_file->setIcon(QIcon(":/images/white_theme/save.png"));
    }
    else
    {
        _list_icon = ":/images/black_theme/list_icon.png";
        new_file->setIcon(QIcon(":/images/black_theme/new.png"));
        open_file->setIcon(QIcon(":/images/black_theme/open.png"));
        save_file->setIcon(QIcon(":/images/black_theme/save.png"));
    }

    for(qint8 i{}; i < _list_widget->count(); ++i)
        _list_widget->item(i)->setIcon(QIcon(_list_icon));
}

void EditCode::CreateMenuBar()
{
    _menuBar = new QMenuBar(this);
    _menuBar->setGeometry(0, 0, this->maximumWidth(), 30);

    _VLayout->setMenuBar(_menuBar);

    new_file = new QAction("&New File", this);
    open_file = new QAction("&Open File", this);
    save_file = new QAction("&Save File", this);
    QAction* quit = new QAction("&Quit", this);
    QAction* run = new QAction("&Run (CTRL+F5)", this);
    QAction* left_form = new QAction("&Left Form", this);
    QAction* right_form = new QAction("&Right Form", this);
    QAction* standart_style = new QAction("&Standart Style" ,this);
    QAction* black_style = new QAction("&Black Style", this);

    QActionGroup* layout_group = new QActionGroup(this);
    layout_group->setExclusive(true);

    right_form->setCheckable(true);
    left_form->setCheckable(true);
    right_form->setActionGroup(layout_group);
    left_form->setActionGroup(layout_group);
    left_form->setChecked(true);
    _layout_form = true;

    QActionGroup* style_group = new QActionGroup(this);
    style_group->setExclusive(true);

    black_style->setCheckable(true);
    standart_style->setCheckable(true);
    black_style->setActionGroup(style_group);
    standart_style->setActionGroup(style_group);
    standart_style->setChecked(true);
    _color_style = true;

    new_file->setShortcut(tr("CTRL+N"));
    open_file->setShortcut(tr("CTRL+O"));
    save_file->setShortcut(tr("CTRL+S"));
    run->setShortcut(tr("CTRL+F5"));
    quit->setShortcut(tr("CTRL+Q"));
    left_form->setShortcut(tr("ALT+0"));
    right_form->setShortcut(tr("ALT+SHIFT+0"));

    connect(quit, &QAction::triggered, this, &QApplication::quit);
    connect(new_file, &QAction::triggered, this, &EditCode::NewFile);
    connect(open_file, &QAction::triggered, this, &EditCode::OpenFile);
    connect(save_file, &QAction::triggered, this, &EditCode::SaveFile);
    connect(run, &QAction::triggered, this, &EditCode::Run);
    connect(left_form, &QAction::triggered, this, [this](){
        if(!_layout_form)
        {
            _layout_form = true;
            setFormApp();
        }
    });
    connect(right_form, &QAction::triggered, this, [this](){
        if(_layout_form)
        {
            _layout_form = false;
            setFormApp();
        }
    });
    connect(standart_style, &QAction::triggered, this, [this](){
        if(!_color_style)
        {
            _color_style = true;
            setStyleApp();
        }
    });
    connect(black_style, &QAction::triggered, this, [this](){
        if(_color_style)
        {
            _color_style = false;
            setStyleApp();
        }
    });

    _file_bar = _menuBar->addMenu("File");
    _menuBar->addAction(run);
    _views_bar = _menuBar->addMenu("Views");

    _file_bar->addAction(new_file);
    _file_bar->addAction(open_file);
    _file_bar->addAction(save_file);
    _file_bar->addSeparator();
    _file_bar->addAction(quit);

    _views_bar->addAction(left_form);
    _views_bar->addAction(right_form);
    _views_bar->addAction(standart_style);
    _views_bar->addAction(black_style);
}

void EditCode::CreateMainApp()
{
    _name_file = new QLabel(this);
    _splitter = new QSplitter(this);
    _edit_code = new QTextEdit(this);
    _number_code = new QTextEdit(this);
    _list_widget = new QListWidget(this);

    _splitter->addWidget(_number_code);
    _splitter->addWidget(_edit_code);
    _splitter->setHandleWidth(0);
    _splitter->setSizes(QList<int>() << 30 << 1800);

    _number_code->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _number_code->setReadOnly(true);

    QTextOption textOption = _edit_code->document()->defaultTextOption();
    textOption.setTabStopDistance(20);
    _edit_code->document()->setDefaultTextOption(textOption);

    _But_Remove = new QPushButton(this);
    _But_All_Remove = new QPushButton(this);

    _But_Remove->setText("Remove File");
    _But_All_Remove->setText("Remove All File");

    _But_Remove->setShortcut(QKeySequence(Qt::Key_Delete));
    _But_All_Remove->setShortcut(QKeySequence(Qt::Key_Delete));

    connect(_list_widget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item){
        if(!_path_file.isEmpty())
            SelectFile(item);
    });
    connect(_list_widget, &QListWidget::itemClicked, [this](QListWidgetItem *item){
        if(remove_file == item)
            remove_file = nullptr;
        else
            remove_file = item;
    });
    connect(_But_Remove, &QPushButton::pressed, this, [this](){
        if(remove_file != nullptr)
        {
            Remove(remove_file->text());

            QListWidgetItem* item = _list_widget->takeItem(_list_widget->row(remove_file));
            delete item;
            remove_file = nullptr;
        }
    });
    connect(_But_All_Remove, &QPushButton::pressed, this, [this](){
        All_Remove();
        remove_file = nullptr;
    });

    _HLayout = new QHBoxLayout{};
    _VLayout_list = new QVBoxLayout{};
    _HLayout_name = new QHBoxLayout{};

    _VLayout_list->addWidget(_list_widget);
    _VLayout_list->addWidget(_But_Remove);
    _VLayout_list->addWidget(_But_All_Remove);

    QSpacerItem* space = new QSpacerItem(0, 1.2 * _VLayout_list->sizeHint().height());
    _VLayout_list->addSpacerItem(space); 

    _HLayout_name->addWidget(_name_file);
    _spacer = new QSpacerItem(_number_code->width() * 1.9, 0);
    _HLayout_name->insertSpacerItem(0, _spacer);

    _VLayout->addLayout(_HLayout_name);
    _VLayout->addLayout(_HLayout);

    _VLayout->setStretchFactor(_HLayout_name, 1);
    _VLayout->setStretchFactor(_HLayout, 50);

    this->setLayout(_VLayout);
}
