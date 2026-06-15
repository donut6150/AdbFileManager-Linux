#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("AdbFileManager");
    
    MainWindow w;
    w.show();
    return QApplication::exec();
}
