#include "buttombuttonsframe.h"
#include "ui_buttombuttonsframe.h"

ButtomButtonsFrame::ButtomButtonsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ButtomButtonsFrame)
{
    ui->setupUi(this);
}

ButtomButtonsFrame::~ButtomButtonsFrame()
{
    delete ui;
}
