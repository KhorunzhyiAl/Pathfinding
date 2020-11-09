#include "menuframe.h"
#include "ui_menuframe.h"

MenuFrame::MenuFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MenuFrame)
{
    ui->setupUi(this);
}

MenuFrame::~MenuFrame()
{
    delete ui;
}
