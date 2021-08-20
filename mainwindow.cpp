#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifndef DESKTOP
    dbg_file = fopen("/home/nemo/qmems.dbg", "w");
    fprintf(dbg_file, "Open dbg session...\n");
    fflush(dbg_file);
#endif
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
    fprintf(dbg_file, "Key press: %X\n", event->key());
    fflush(dbg_file);
#endif
}
