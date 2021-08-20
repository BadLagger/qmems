#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
#ifdef DESKTOP
    qDebug() << "Key: " << event->text() << event->key();
    if (event->key() == Qt::Key_Escape)
        QApplication::quit();
#else
    static FILE *file = nullptr;

    if(file == nullptr) {
        file = fopen("/home/nemo/qmems.dbg", "w");
    }
    fprintf(file, "Key press: %X\n", event->key());
    fflush(file);
#endif
}
