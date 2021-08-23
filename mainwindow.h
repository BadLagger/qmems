#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    Ui::MainWindow *ui;
    QTimer timer;
    int file_count;
    unsigned long long count;
    QString file_name;

    bool start;
    void keyPressEvent(QKeyEvent *event);
    void updateTime(unsigned long long time_ms);
    void updateSizeInfo();

private slots:
    void onClick();
    void onTimer();
};

#endif // MAINWINDOW_H
