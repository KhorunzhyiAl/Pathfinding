#ifndef VIRTUALMAP_H
#define VIRTUALMAP_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QDebug>
#include <QMutex>
#include "global.h"


class VirtualMap : public QObject
{
    Q_OBJECT

public:
    VirtualMap();

    void initialize();
    void clear();
    NODE at(QPoint pos);
    const QVector<QVector<NODE>>& getMap();
    void clearPath();
    bool IsLocked();

    bool isInstantUpdateMode = false;

signals:
    void nodeChanged(QPoint pos, NODE newType);
    void mapChanged();
    void pathChanged(); // called when the path is drawn and something has changed so the path should be redrawn

public slots:
    /*
     * Mouse event slots update image and virtual map based on the button pressed and
     * cursor's position. They do one of the following:
     * 1. add wall node
     * 2. erase wall node
     * 3. move start/finish node.
    */
    void mousePressed(QPoint pos, Qt::MouseButton button);
    void mouseMovedWhilePressed(QPoint pos);
    void mouseReleased();

    void addNode(QPoint pos, NODE type);
    void lockForUser();
    void unlockChanges();
    void onFinishedDrawingPath();

private:
    bool isLockedForUser = false;
    bool isErasing = false;
    bool isDruggingNode = false; // true, if user is drugging start or finish node
    QPoint prevDraggingNodePos = QPoint(-1,-1); // prewious possition of the node that is being dragged

    /* When node is moving it may overlap with another one. After it's moved away, we need to replace
     * it's previous position with the node that was there before */
    NODE overlapsWith = empty;

    QVector<QVector<NODE>> map;
    QMutex mutex;

    void DragNode(QPoint newPos);
};

#endif // VIRTUALMAP_H
