#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    count(0),
    file_count(1),
    start(false)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 16777220) {
        ui->pushButton->animateClick();
    }
#ifdef DESKTOP
    else if (event->key() == Qt::Key_Escape)
        QApplication::quit();
#endif
}

void MainWindow::onClick()
{
    if(!start){
        ui->RecLbl->setText(QString::fromUtf8("ЗАПИСЬ ИДЁТ"));
        ui->pushButton->setText(QString::fromUtf8("СТОП"));
        count = 0;
        ui->TimerLbl->setText("00:00:00:000");
        file_name = "mems_" + QString::number(file_count++) + ".log";
        ui->FileNameLbl->setText(file_name);
#ifndef DESKTOP
        QString cmd = "dbus-send --session --print-reply --dest=sn.ornap.nvsd /navsensor sn.ornap.nvsd.NavSensor.Compass.LogControl string:'on' string:'" + file_name + "'";
        QByteArray cmd_array = cmd.toLocal8Bit();
        const char *c_str_cmd = cmd_array.data();
        system(c_str_cmd);
#endif
        timer.start(10);
        start = true;
    } else {
        ui->RecLbl->setText(QString::fromUtf8("ЗАПИСЬ НЕ ИДЁТ"));
        ui->pushButton->setText(QString::fromUtf8("СТАРТ"));
#ifndef DESKTOP
        QString cmd = "dbus-send --session --print-reply --dest=sn.ornap.nvsd /navsensor sn.ornap.nvsd.NavSensor.Compass.LogControl string:'off' string:'" + file_name + "'";
        QByteArray cmd_array = cmd.toLocal8Bit();
        const char *c_str_cmd = cmd_array.data();
        system(c_str_cmd);
#endif
        timer.stop();
        start = false;
    }
}

void MainWindow::onTimer()
{
    count += 10;
    updateTime(count);
}

void MainWindow::updateTime(unsigned long long time_ms)
{
    char str[13] = {0};
    int ms   = time_ms % 1000;
    int sec  = (unsigned long long)(time_ms / 1000) % 60;
    int min  = (unsigned long long)(time_ms / 60000) % 60;
    int hour = (unsigned long long)(time_ms / 3600000) % 24;

    sprintf(str, "%02d:%02d:%02d:%03d", hour, min, sec, ms);
    ui->TimerLbl->setText(str);
#ifndef DESKTOP
    QString full_file_path = "/mnt/mmc0/" + file_name;
    QFile log_file(full_file_path);
    if(log_file.open(QIODevice::ReadOnly)) {
        QString file_size_str = QString::number(log_file.size()) + QString::fromUtf8(" Байт");
        log_file.close();
    }
#endif
}
