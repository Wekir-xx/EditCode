#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow EditCode;

    EditCode.showMaximized();

    return a.exec();
}
