#ifndef BUTTOMBUTTONSFRAME_H
#define BUTTOMBUTTONSFRAME_H

#include <QFrame>

namespace Ui {
class ButtomButtonsFrame;
}

class ButtomButtonsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ButtomButtonsFrame(QWidget *parent = nullptr);
    ~ButtomButtonsFrame();


    Ui::ButtomButtonsFrame *ui;

private:
};

#endif // BUTTOMBUTTONSFRAME_H
