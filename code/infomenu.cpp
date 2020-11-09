#include "infomenu.h"
#include "ui_infomenu.h"

infomenu::infomenu(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::infomenu)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::WindowType::Tool);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::NoDropShadowWindowHint);

}

infomenu::~infomenu()
{
    delete ui;
}
