#ifndef EDITCODE_H
#define EDITCODE_H

#include <TextEditObserver.h>
#include <HighLighter.h>

#include <QWidget>
#include <QString>
#include <QMenuBar>
#include <QLabel>
#include <QApplication>
#include <QLineEdit>
#include <QList>
#include <QtWidgets/QListWidget>
#include <QPushButton>
#include <QDialog>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>

class EditCode : public QWidget
{
    Q_OBJECT

public:
    EditCode(QWidget *parent = nullptr);
    ~EditCode();

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
    void setFormApp();
    void setStyleApp();

private:
    QString GetNameFile(size_t number_file) const;
    void PushFile(QString pathfile);
    void CMD(QStringList& list);
    void ChangeImage();
    void CreateMenuBar();
    void CreateMainApp();

private:
    HighLighter* _lighter;
    TextEditObserver* _observer;

private:
    QVBoxLayout* _VLayout;
    QHBoxLayout* _HLayout;
    QVBoxLayout* _VLayout_list;
    QHBoxLayout* _HLayout_name;
    QSpacerItem* _spacer;

private:
    QMenuBar* _menuBar;
    QMenu* _file_bar;
    QMenu* _views_bar;
    QDialog* _dialog_newfile;
    bool _layout_form;
    bool _color_style;

private:
    QString _list_icon;
    QAction* new_file;
    QAction* open_file;
    QAction* save_file;

private:
    QSplitter* _splitter;
    QLabel* _name_file;
    QTextEdit* _edit_code;
    QTextEdit* _number_code;

private:
    QListWidget* _list_widget;
    QListWidgetItem* remove_file{};
    QPushButton* _But_Remove;
    QPushButton* _But_All_Remove;

private:
    QList<QString> _path_file;
    QList<QString> _file_code;
    size_t _index;
};

#endif // EDITCODE_H
