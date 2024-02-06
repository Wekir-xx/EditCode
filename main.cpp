#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow EditCode;
    EditCode.showMaximized();
    EditCode.show();

    return a.exec();
}
