#include "EditCode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EditCode EditCode{};

    EditCode.showMaximized();

    return a.exec();
}
