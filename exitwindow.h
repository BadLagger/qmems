#ifndef EXITWINDOW_H
#define EXITWINDOW_H

#include <QWidget>

namespace Ui {
class ExitWindow;
}

class ExitWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ExitWindow(QWidget *parent = nullptr);
    ~ExitWindow();

signals:
    void sendCancelBtnEvent();
    void sendConfirmBtnEvent();
private:
    Ui::ExitWindow *ui;

    void keyPressEvent(QKeyEvent *event);
private slots:
    void onYesClick();
    void onNoClick();
};

#endif // EXITWINDOW_H
