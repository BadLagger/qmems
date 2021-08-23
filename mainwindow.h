#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    struct Delay : private QThread{
        static void msleep(unsigned long msecs) { QThread::msleep(msecs); }
    };

    static const QString ChargeFilePath;
    Ui::MainWindow *ui;
    QTimer timer;
    QTimer upd_charge_lvl_timer;
    unsigned long long count;
    int file_count;
    QString file_name;

    bool start;
    void keyPressEvent(QKeyEvent *event);
    void updateTime(unsigned long long time_ms);
    void updateSizeInfo();

private slots:
    void onClick();
    void onTimer();
    void updateChargeLevel();
};

#endif // MAINWINDOW_H
