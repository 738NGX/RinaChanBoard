#include <QApplication>
#include <QPushButton>

#include "rinatesterwindow.h"

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath(R"(D:\Qt\6.8.0\mingw_64\plugins)");
    QApplication a(argc, argv);

    RinaTesterWindow w;
    w.setWindowTitle("璃奈板测试平台");
    //const QIcon icon("./assets/icon.ico");
    //w.setWindowIcon(icon);
    w.show();

    return QApplication::exec();
}
