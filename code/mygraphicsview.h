#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
private:
    bool mousePressed = false;
    qreal theScale = 1;

private slots:
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    MyGraphicsView();

signals:
    void mousePressedInDrawMode(QPoint pos, Qt::MouseButton);
    void mouseMovedInDrawMode(QPoint pos);
    void mouseReleasedInDrawMode();

    void scaled(qreal currentScale);
};

#endif // MYGRAPHICSVIEW_H
