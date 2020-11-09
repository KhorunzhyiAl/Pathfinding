#ifndef MENUFRAME_H
#define MENUFRAME_H

#include <QFrame>

namespace Ui {
class MenuFrame;
}

class MenuFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MenuFrame(QWidget *parent = nullptr);
    ~MenuFrame();

    Ui::MenuFrame *ui;

private:
};

#endif // MENUFRAME_H
