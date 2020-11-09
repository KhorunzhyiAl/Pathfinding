#include "mygraphicsview.h"


MyGraphicsView::MyGraphicsView()
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setTransformationAnchor(ViewportAnchor::AnchorViewCenter);
    this->setStyleSheet("border: 0;");

    theScale = 0.2;
    this->scale(theScale, theScale);
    emit scaled(theScale);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    this->setTransformationAnchor(ViewportAnchor::AnchorUnderMouse);
    if (event->delta() > 0 && theScale < 2) {
        this->resetMatrix();
        theScale += 0.05;
        this->scale(theScale, theScale);
        emit scaled(theScale);
    } else if (event->delta() < 0 && theScale > 0.01) {
        this->resetMatrix();
        theScale -= 0.05;
        this->scale(theScale, theScale);
        emit scaled(theScale);
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (this->dragMode() == QGraphicsView::DragMode::NoDrag) {
        mousePressed = true;
        emit mousePressedInDrawMode(mapToScene(event->pos()).toPoint(), event->button());
    }

    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePressed)
        emit mouseMovedInDrawMode(mapToScene(event->pos()).toPoint());

    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->dragMode() == QGraphicsView::DragMode::NoDrag) {
        mousePressed = false;
        emit mouseReleasedInDrawMode();
    }

    QGraphicsView::mouseReleaseEvent(event);
}
