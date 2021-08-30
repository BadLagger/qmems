#include "exitwindow.h"
#include "ui_exitwindow.h"
#include <QKeyEvent>

ExitWindow::ExitWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExitWindow)
{
    ui->setupUi(this);
    connect(ui->YesBtn, SIGNAL(clicked()), this, SLOT(onYesClick()));
    connect(ui->NoBtn, SIGNAL(clicked()), this, SLOT(onNoClick()));
}

ExitWindow::~ExitWindow()
{
    delete ui;
}

void ExitWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 16777220) {     // Enter key - ↵
        ui->YesBtn->animateClick();
    } else if (event->key() == 16777216) { // Close key - X
        ui->NoBtn->animateClick();
    }
}

void ExitWindow::onYesClick()
{
    emit sendConfirmBtnEvent();
}

void ExitWindow::onNoClick()
{
    emit sendCancelBtnEvent();
}
