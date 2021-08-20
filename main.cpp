#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.setCursor(Qt::BlankCursor);
    w.show();

    return a.exec();
}
