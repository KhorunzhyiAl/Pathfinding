#include "virtualmap.h"

VirtualMap::VirtualMap()
{
    initialize();
}

void VirtualMap::addNode(QPoint pos, NODE type)
{
    if (pos.x() >= X_NODES ||
            pos.y() >= Y_NODES ||
            pos.x() < 0 ||
            pos.y() < 0) {
        return;
    }

    if (at(pos) == type)
        return;

    if (at(pos) == start || at(pos) == finish) {
        return;
    }

    if (isInstantUpdateMode) {
        // we don't wan't user to erase the path drawn by algorithm
        if (type == empty && (at(pos) == visited || at(pos) == path)) {
            return;
        }

        // in instant update mode animation shouldn't be called when algorithm adds a new node
        if (type == visited || type == path) {
            map[pos.x()][pos.y()] = type;
            return;
        }

        // start and finish nodes should still be animated even in instant update mode
        if (type == start || type == finish) {
            map[pos.x()][pos.y()] = type;
            emit nodeChanged(pos, type);
            return;
        }

        // if we add a node that may change the path, we need to ask the algorithm to reprocess and redraw it
        if (at(pos) == visited || at(pos) == path ||
                (type == empty && at(pos) == wall))
        {
            map[pos.x()][pos.y()] = type;
            clearPath();

            if (type != empty)
                emit nodeChanged(pos, type);

            emit pathChanged();
            return;
        }
    }

    map[pos.x()][pos.y()] = type;
    emit nodeChanged(pos, type);
}

void VirtualMap::initialize()
{
    QVector<NODE> column(Y_NODES, NODE::empty);
    map.fill(column, X_NODES);
    map[int(X_NODES * 0.25)][int(Y_NODES * 0.5)] = NODE::start;
    map[int(X_NODES * 0.75)][int(Y_NODES * 0.5)] = NODE::finish;
}

void VirtualMap::clear()
{
    isInstantUpdateMode = false;
    overlapsWith = NODE::empty;

    for (int i = 0; i < X_NODES; i++) {
        for (int j = 0; j < Y_NODES; j++) {
            if (map[i][j] != start && map[i][j] != finish) {
                map[i][j] = empty;
            }
        }
    }

    emit mapChanged();
}

NODE VirtualMap::at(QPoint pos)
{
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= X_NODES || pos.y() >= Y_NODES)
        return empty;
    return map[pos.x()][pos.y()];
}

const QVector<QVector<NODE> > &VirtualMap::getMap()
{
    return this->map;
}

void VirtualMap::clearPath()
{
    for (int i = 0; i < X_NODES; i++) {
        for (int j = 0; j < Y_NODES; j++) {
            if (map[i][j] == NODE::visited || map[i][j] == NODE::path) {
                map[i][j] = NODE::empty;
                emit nodeChanged(QPoint(i, j), empty);
            }
        }
    }
//    emit mapChanged();
}

bool VirtualMap::IsLocked()
{
    return  isLockedForUser;
}

void VirtualMap::lockForUser()
{
    isLockedForUser = true;
}

void VirtualMap::unlockChanges()
{
    isLockedForUser = false;
}

void VirtualMap::onFinishedDrawingPath()
{
    isLockedForUser = false;
    isInstantUpdateMode = true;
}

void VirtualMap::mousePressed(QPoint pos, Qt::MouseButton button)
{
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= X_NODES || pos.y() >= Y_NODES) {
        return;
    }

    if (isLockedForUser) {
        return;
    }

    if (button == Qt::MouseButton::RightButton) {
        this->addNode(pos, NODE::empty);
        isErasing = true;
    } else if (button == Qt::MouseButton::LeftButton) {
        if (this->at(pos) == NODE::start || this->at(pos) == NODE::finish) {
            isDruggingNode = true;
        } else {
            this->addNode(pos, NODE::wall);
        }
    }
}

void VirtualMap::mouseMovedWhilePressed(QPoint pos)
{
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= X_NODES || pos.y() >= Y_NODES) {
        return;
    }

    if (isLockedForUser) {
        return;
    }

    if (isDruggingNode) {
        DragNode(pos);
    }

    if (at(pos) == NODE::start || at(pos) == NODE::finish) {
        return;
    } else {
        if (isErasing) {
            this->addNode(pos, NODE::empty);
        } else {
            this->addNode(pos, NODE::wall);
        }
    }

}

void VirtualMap::mouseReleased()
{
    isDruggingNode = false;
    isErasing = false;
    prevDraggingNodePos = QPoint(-1, -1);
}

void VirtualMap::DragNode(QPoint newPos)
{
    if (newPos == prevDraggingNodePos || prevDraggingNodePos == QPoint(-1, -1)) {
        prevDraggingNodePos = newPos;
        return;
    }

    if (at(newPos) == start || at(newPos) == finish) {
        return;
    }

    NODE temp = at(newPos);
    if (temp == path || temp == visited)
        temp = empty;

//    addNode(newPos, at(prevDraggingNodePos));
//    addNode(prevDraggingNodePos, overlapsWith);
    map[newPos.x()][newPos.y()] = at(prevDraggingNodePos);
    emit nodeChanged(newPos, at(prevDraggingNodePos));

    map[prevDraggingNodePos.x()][prevDraggingNodePos.y()] = overlapsWith;
    emit nodeChanged(prevDraggingNodePos, overlapsWith);

    if (isInstantUpdateMode) {
        clearPath();
        emit pathChanged();
    }

    overlapsWith = temp;
    prevDraggingNodePos = newPos;
}
