#include "popupmenu.h"
#include "ui_popupmenu.h"

popupmenu::popupmenu(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::popupmenu)
{
    ui->setupUi(this);
}

popupmenu::~popupmenu()
{
    delete ui;
}
