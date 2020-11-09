#include "astaralgorithm.h"

AStarAlgorithm::AStarAlgorithm(VirtualMap *vm)
    : PathfindingAlgorithm(vm)
{

}

AStarAlgorithm::~AStarAlgorithm()
{
    opened.clear();
    closed.clear();
}


void AStarAlgorithm::onStart()
{
    resetMap();
}

void AStarAlgorithm::onStop()
{

}

void AStarAlgorithm::step()
{
    if (opened.size() == 0) {
        onPathFinished();
        return;
    }

    // find a node in open with the lowest f-cost
    auto it = opened.begin();
    Node current = (*it);
    it++;
    while (it != opened.end()) {
        if (it->f < current.f || ((it->f == current.f) && (it->h < current.h))) {
            current = (*it);
        }

        it++;
    }


    opened.remove(PointToPair(current.pos));
    closed.insert(PointToPair(current.pos), current);

    if (vm->at(current.pos) == NODE::finish) {
        onStepsFinished();
        return;
    }

    vm->addNode(current.pos, NODE::visited);

    int x = current.pos.x();
    int y = current.pos.y();
    assignNeighbour(QPoint(x+1, y  ), current);
    assignNeighbour(QPoint(x,   y-1), current);
    assignNeighbour(QPoint(x-1, y  ), current);
    assignNeighbour(QPoint(x,   y+1), current);
}

void AStarAlgorithm::PathStep()
{
    if (vm->at(pathWalker) == NODE::start) {
        onPathFinished();
        return;
    }

    pathWalker = closed.find(PointToPair(pathWalker))->parentPos;
    vm->addNode(pathWalker, NODE::path);
}

void AStarAlgorithm::assignNeighbour(QPoint pos, Node current)
{
    if (vm->at(pos) == NODE::wall || closed.find(PointToPair(pos)) != closed.end()) {
        return;
    }
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= X_NODES || pos.y() >= Y_NODES) {
        return;
    }

    auto nei = opened.find(PointToPair(pos));
    if (nei == opened.end()) {
        nei = opened.insert(PointToPair(pos), Node(pos, current.pos));
    }

    if (nei->g > current.g + 1) {
        nei->g = current.g + 1;

//        qDebug() << "pos: " << pos << "\tfinishpos: " << finishPos;
        int hx = finishPos.x() - pos.x();
//        qDebug() << "hx: " << hx;
        int hy = finishPos.y() - pos.y();
//        qDebug() << "hy: " << hy;
        if (hx < 0) hx = -hx;
        if (hy < 0) hy = -hy;
        int h = hx + hy;
//        qDebug() << "h: " << h;

        nei->h = h;
        nei->f = nei->g + h;

        nei->parentPos = current.pos;
//        qDebug() << "node at pos: " << pos << "\tf = " << nei->f;
    }
}

void AStarAlgorithm::resetMap()
{
    opened.clear();
    closed.clear();

    for (int x = 0; x < X_NODES; x++) {
        for (int y = 0; y < Y_NODES; y++) {
            if (vm->at(QPoint(x,y)) == NODE::finish) {
                this->finishPos = QPoint(x,y);
                this->pathWalker = this->finishPos;
            } else if (vm->at(QPoint(x,y)) == NODE::start) {
                QPoint startPos = QPoint(x,y);
                Node n = Node(startPos, startPos);
                n.g = 0;

                int h = startPos.x() - x + startPos.y() - y;
                if (h < 0) h = -h;
                n.h = h;

                n.f = n.g + n.h;

                opened.insert(PointToPair(startPos), n);
            }
        }
    }
}

QPair<int, int> AStarAlgorithm::PointToPair(const QPoint &point)
{
    return QPair<int, int>(point.x(), point.y());
}

AStarAlgorithm::Node::Node(QPoint pos, QPoint parent)
    : pos(pos), parentPos(parent)
{

}

AStarAlgorithm::Node::Node()
    : pos(QPoint(0,0)), parentPos(QPoint(0,0))
{

}

AStarAlgorithm::Node::Node(const AStarAlgorithm::Node &other)
{
    this->f = other.f;
    this->g = other.g;
    this->h = other.h;
    this->pos = other.pos;
    this->parentPos = other.parentPos;
}

AStarAlgorithm::Node &AStarAlgorithm::Node::operator=(const AStarAlgorithm::Node &other)
{
    this->f = other.f;
    this->g = other.g;
    this->h = other.h;
    this->pos = other.pos;
    this->parentPos = other.parentPos;

    return *this;
}
