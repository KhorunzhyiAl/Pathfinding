#ifndef INFOMENU_H
#define INFOMENU_H

#include <QFrame>

namespace Ui {
class infomenu;
}

class infomenu : public QFrame
{
    Q_OBJECT

public:
    explicit infomenu(QWidget *parent = nullptr);
    ~infomenu();

private:
    Ui::infomenu *ui;
};

#endif // INFOMENU_H
