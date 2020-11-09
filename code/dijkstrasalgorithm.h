#ifndef DIJKSTRASALGORITHM_H
#define DIJKSTRASALGORITHM_H

#include <QList>
#include "PathfindingAlgorithm.h"
#include "global.h"


class DijkstrasAlgorithm : public PathfindingAlgorithm
{
public:
    DijkstrasAlgorithm(VirtualMap *vm);
    ~DijkstrasAlgorithm() override;

public slots:
    void onStop() override;
    void step() override;


protected slots:
    void PathStep() override;
    void onStart() override;


private:
    struct Vert
    {
        NODE type;
        QPoint pos;
        QPoint prev;
        int dist;
        bool isVisited = false;

        Vert(NODE type, QPoint pos, int dist) : type(type), pos(pos), dist(dist) { prev = QPoint(-1, -1); }
    };

    QPoint start;
    QPoint finish;
    QVector<QVector<Vert>> map;
    QPoint pathWalker;
    const QVector<QVector<NODE>> &nodeMap;

    /*
     * instead of storing all unvisited vertexes most of which would have infinite distance value and would never be used,
     * we want to store only those that we know the distance of.
    */
    QList<Vert*> activeVerts;

    void TrySetVertexDistance(QPoint pos, QPoint prevVertexPos);
    void ResetMap();
};

#endif // DIJKSTRASALGORITHM_H
