#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

const QString MainWindow::ChargeFilePath = "/sys/devices/platform/3802c000.i2c/i2c-0/0-0055/power_supply/bq27531-0/capacity";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    count(0),
    file_count(1),
    start(false),
    key_press(false)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect(&upd_charge_lvl_timer, SIGNAL(timeout()), this, SLOT(updateChargeLevel()));
    connect(&key_timer, SIGNAL(timeout()), this, SLOT(onKeyTimeout()));
    upd_charge_lvl_timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!key_press) {
        if (event->key() == 16777220) {
            ui->pushButton->animateClick();
        }
#ifdef DESKTOP
        else if (event->key() == Qt::Key_Escape)
            QApplication::quit();
#endif
        key_press = true;
        key_timer.start(400);
    }
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
        Delay::msleep(500);
        updateSizeInfo();
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
    int sec  = (time_ms / 1000) % 60;
    int min  = (time_ms / 60000) % 60;
    int hour = int(time_ms / 3600000);

    sprintf(str, "%02d:%02d:%02d:%03d", hour, min, sec, ms);
    ui->TimerLbl->setText(str);
    updateSizeInfo();
}

void MainWindow::onKeyTimeout()
{
    key_timer.stop();
    key_press = false;
}

void MainWindow::updateChargeLevel()
{
#ifndef DESKTOP
    QFile charge_file(ChargeFilePath);
    if (charge_file.open(QIODevice::ReadOnly)) {
        QByteArray data = charge_file.readAll();
        data = data.trimmed();
        if ((data.toInt() > 0) && (data.toInt() <= 100))
            ui->ChargeLvlLbl->setText(QString::number(data.toInt()) + QString(" %"));
        else
            ui->ChargeLvlLbl->setText(QString::fromUtf8("нз=") + QString::number(data.toInt()));
    } else {
        ui->ChargeLvlLbl->setText(QString::fromUtf8("недоступно"));
    }
#endif
}

void MainWindow::updateSizeInfo(void)
{
#ifndef DESKTOP
    QString full_file_path = "/mnt/mmc0/" + file_name;
    QFile log_file(full_file_path);
    if(log_file.open(QIODevice::ReadOnly)) {
        quint64 file_size = log_file.size();
        log_file.close();

        QString file_size_str = QString::fromUtf8("нз=") + QString::number(file_size);
        if (file_size < 1024) {
            file_size_str = QString::number(file_size) + QString::fromUtf8(" байт");
        } else 
            if (file_size < 1048576) {
                file_size_str = QString::number(file_size/1024., 'f', 2) + QString::fromUtf8(" Кбайт");
            } else
                if (file_size < 1073741824) {
                    file_size_str = QString::number(file_size/1048576., 'f', 2) + QString::fromUtf8(" Мбайт");
                } else
                    if (file_size < 1099511627776) {
                      file_size_str = QString::number(file_size/1073741824., 'f', 2) + QString::fromUtf8(" Гбайт");  
                    }
        ui->FileSizeLbl->setText(file_size_str);
    } else {
        ui->FileSizeLbl->setText(QString::fromUtf8("недоступно"));
    }
#endif
}
