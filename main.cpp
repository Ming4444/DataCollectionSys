#include "Headerfiles.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    MainWindow w;
    if(!w.initFlag) return -1;
    w.show();
    return a.exec();
}
