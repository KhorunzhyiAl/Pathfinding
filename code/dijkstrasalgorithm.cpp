#include "dijkstrasalgorithm.h"


DijkstrasAlgorithm::DijkstrasAlgorithm(VirtualMap *vm)
    : PathfindingAlgorithm(vm), nodeMap(vm->getMap())
{

}

DijkstrasAlgorithm::~DijkstrasAlgorithm()
{
    timerStep->stop();
    timerPath->stop();
    isInstantUpdateMode = false;
}

void DijkstrasAlgorithm::ResetMap()
{   
    map.clear();

    map.reserve(X_NODES);
    for (int i = 0; i < X_NODES; i++) {
        QVector<Vert> column;
        column.reserve(Y_NODES);
        for (int j = 0; j < Y_NODES; j++) {
            NODE current = nodeMap[i][j];

            Vert newVert(current, QPoint(i, j), INT32_MAX);

            column.append(newVert);

            if (current == NODE::start) {
                start = QPoint(i,j);
            } else if (current == NODE::finish) {
                finish = QPoint(i,j);
            }
        }
        map.append(column);
    }


    map[start.x()][start.y()].dist = 0;
    map[start.x()][start.y()].isVisited = true;

    activeVerts.clear();
    activeVerts.append(&map[start.x()][start.y()]);
}

void DijkstrasAlgorithm::step()
{
    if (activeVerts.size() == 0) {
        onPathFinished();
        return;
    }

    // find an active vertex with the smallest distance value
    Vert* smallestDistNode = activeVerts.first();
    for (Vert* v : activeVerts) {
        if (v->dist < smallestDistNode->dist) {
            smallestDistNode = v;
        }
    }


    // visit all its neighbours (four surrounding cells) and set their distance value.
    // It doesn't matter if some of them are already visited, their distance won't change since they already have the smallest possible.
    int x = smallestDistNode->pos.x();
    int y = smallestDistNode->pos.y();

    TrySetVertexDistance(QPoint(x,   y+1), QPoint(x,y));
    TrySetVertexDistance(QPoint(x+1, y),   QPoint(x,y));
    TrySetVertexDistance(QPoint(x,   y-1), QPoint(x,y));
    TrySetVertexDistance(QPoint(x-1, y),   QPoint(x,y));

    activeVerts.removeOne(smallestDistNode);
}

void DijkstrasAlgorithm::TrySetVertexDistance(QPoint pos, QPoint prevVertexPos)
{
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= X_NODES || pos.y() >= Y_NODES) {
        return;
    }

    if (map[pos.x()][pos.y()].type == wall) {
        return;
    }


    int prevVertexDistance = map[prevVertexPos.x()][prevVertexPos.y()].dist;

    if (map[pos.x()][pos.y()].dist > prevVertexDistance + 1) {
        map[pos.x()][pos.y()].dist = prevVertexDistance + 1;

        if (map[pos.x()][pos.y()].type == NODE::finish) {
            activeVerts.clear();
            map[pos.x()][pos.y()].isVisited = true;
            map[pos.x()][pos.y()].prev = prevVertexPos;

            pathWalker = finish;

            onStepsFinished();

            return;
        }

        if (!map[pos.x()][pos.y()].isVisited) {
            activeVerts.append(&map[pos.x()][pos.y()]);

            map[pos.x()][pos.y()].isVisited = true;
            map[pos.x()][pos.y()].prev = prevVertexPos;

            vm->addNode(pos, NODE::visited);
        }
    }
}


void DijkstrasAlgorithm::PathStep()
{
    QPoint pw = map[pathWalker.x()][pathWalker.y()].prev;
    Vert current = map[pw.x()][pw.y()];

    pathWalker = pw;

    if (current.type == NODE::start) {
        onPathFinished();
        return;
    }

    vm->addNode(pw, NODE::path);
}

void DijkstrasAlgorithm::onStart()
{
    ResetMap();
}

void DijkstrasAlgorithm::onStop()
{

}

