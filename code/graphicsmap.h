#ifndef SCENEMAP_H
#define SCENEMAP_H

#include <QObject>
#include <QImage>
#include <QPoint>
#include <QPainter>
#include <QList>
#include <QDebug>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QMutex>
#include "virtualmap.h"

struct AnimationInfo
{
    int iter = 0;
    QPoint pos;
    NODE type;

    AnimationInfo(QPoint pos, NODE type) : pos(pos), type(type) {}
};

class GraphicsMap : public QObject
{
    Q_OBJECT

public:
    GraphicsMap();
    ~GraphicsMap();

    VirtualMap *vm;
    const QImage* getImage();

public slots:
    // These functions process mouse actions happenning over the map. For now they just call corresponding functions of vm
    void mousePressed(QPoint globalPos, Qt::MouseButton button);
    void mouseMovedWhilePressed(QPoint globalPos);
    void mouseReleased();

    void onNodeChanged(QPoint pos, NODE newType);
    void onMapChanged();
    void onPathfindingFinished();

    void setAnimationInterval(int ms);

private:
    QImage *image;
    QPainter *painter;


    bool animationStopped = false;
    int interval = 20;
    QMutex mutex;

    QVector<QImage> wallAnim;
    QVector<QImage> startAnim;
    QVector<QImage> finishAnim;
    QVector<QImage> visitedAnim;
    QVector<QImage> pathAnim;

    QList<AnimationInfo> activeAnimations;
    void animationLoop();

    void DrawMap(); // draws the whole image
    void UpdateNode(QPoint pos); // instantly updates one node on the image. pos - node's coordinates on virtual map (not on the image)
    void AddNode(QPoint pos, NODE type); // adds node to virtual map and displays it on image (with animation)


    /*
     * translate coordinates between global scene (image)
     * and virtual map coordiantes (node cell).
    */
    QPoint mapFromGlobal(QPoint pos);
    QPoint mapToGlobal(QPoint pos);
};

#endif // SCENEMAP_H
