#include "EditCode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EditCode window{};

    window.showMaximized();

    return a.exec();
}
