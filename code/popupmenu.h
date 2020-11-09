#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <QFrame>

namespace Ui {
class popupmenu;
}

class popupmenu : public QFrame
{
    Q_OBJECT

public:
    explicit popupmenu(QWidget *parent = nullptr);
    ~popupmenu();

private:
    Ui::popupmenu *ui;
};

#endif // POPUPMENU_H
