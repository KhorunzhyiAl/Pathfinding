#ifndef ASTARALGORITHM_H
#define ASTARALGORITHM_H

#include <QHash>
#include <QPair>
#include <QList>
#include <QVector>
#include "PathfindingAlgorithm.h"
#include "global.h"

class AStarAlgorithm : public PathfindingAlgorithm
{
public:
    AStarAlgorithm(VirtualMap *vm);
    ~AStarAlgorithm();

public slots:
    void onStart() override;
    void onStop() override;
    void step() override;

protected slots:
    void PathStep() override;

private:
    struct Node {
        int h = INT32_MAX;
        int g = INT32_MAX;
        int f = INT32_MAX;

        QPoint pos;
        QPoint parentPos;

        Node(QPoint pos, QPoint parentPos);
        Node();
        Node(const Node& other);
        Node &operator=(const Node& other);
    };

    void assignNeighbour(QPoint pos, Node current);
    void resetMap();
    QPair<int, int> PointToPair(const QPoint& point);
    

    QHash<QPair<int,int>, Node> opened;
    QHash<QPair<int,int>, Node> closed;

    QPoint finishPos;
    QPoint pathWalker;
};

#endif // ASTARALGORITHM_H
