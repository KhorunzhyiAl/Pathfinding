#include "graphicsmap.h"

GraphicsMap::GraphicsMap()
{
    QString path = ":/animations/nodeAnimations/wall/";

    for (int i = 0; true; i++) {
        QString fullPath = path + QString::number(i) + ".bmp";
        QImage frame(fullPath);
        if (frame.isNull()) {
            break;
        }
        wallAnim.append(frame);
    }

    path = ":/animations/nodeAnimations/start/";
    for (int i = 0; true; i++) {
        QImage frame(path + QString::number(i) + ".bmp");
        if (frame.isNull()) {
            break;
        }
        startAnim.append(frame);
    }

    path = ":/animations/nodeAnimations/finish/";
    for (int i = 0; true; i++) {
        QImage frame(path + QString::number(i) + ".bmp");
        if (frame.isNull()) {
            break;
        }
        finishAnim.append(frame);
    }

    path = ":/animations/nodeAnimations/visited/";
    for (int i = 0; true; i++) {
        QImage frame(path + QString::number(i) + ".bmp");
        if (frame.isNull()) {
            break;
        }
        visitedAnim.append(frame);
    }

    path = ":/animations/nodeAnimations/path/";
    for (int i = 0; true; i++) {
        QImage frame(path + QString::number(i) + ".bmp");
        if (frame.isNull()) {
            break;
        }
        pathAnim.append(frame);
    }

    vm = new VirtualMap();
    connect(vm, SIGNAL(nodeChanged(QPoint, NODE)), this, SLOT(onNodeChanged(QPoint, NODE)));
    connect(vm, SIGNAL(mapChanged()), this, SLOT(onMapChanged()));

    image = new QImage(X_NODES * CELL_SIZE, Y_NODES * CELL_SIZE, QImage::Format::Format_RGB16);
    painter = new QPainter(image);
    painter->setRenderHints(QPainter::RenderHint::HighQualityAntialiasing | QPainter::RenderHint::LosslessImageRendering);

    QtConcurrent::run(this, &GraphicsMap::animationLoop);

    DrawMap();
}

GraphicsMap::~GraphicsMap()
{
    this->animationStopped = true;
}

const QImage* GraphicsMap::getImage()
{
    return this->image;
}

void GraphicsMap::mousePressed(QPoint globalPos, Qt::MouseButton button)
{
    QPoint mpos = mapFromGlobal(globalPos);   
    vm->mousePressed(mpos, button);
}

void GraphicsMap::mouseMovedWhilePressed(QPoint globalPos)
{
    QPoint mpos = mapFromGlobal(globalPos);
    vm->mouseMovedWhilePressed(mpos);
}

void GraphicsMap::mouseReleased()
{
    vm->mouseReleased();
}

void GraphicsMap::onNodeChanged(QPoint pos, NODE newType)
{
    AddNode(pos, newType);
}

void GraphicsMap::onMapChanged()
{
    DrawMap();
}

void GraphicsMap::onPathfindingFinished()
{
    vm->onFinishedDrawingPath();
    DrawMap();
}

void GraphicsMap::setAnimationInterval(int ms)
{
    if (ms < 10) ms = 10;
    if (ms > 100) ms = 100;
    this->interval = ms;
}

void GraphicsMap::animationLoop()
{
    while (!animationStopped) {

        mutex.lock();
        QList<AnimationInfo>::iterator it = activeAnimations.begin();

        // remove the animations we don't need. (I use a separate loop because the program crashes otherwise)
        while (it != activeAnimations.end()) {
            if (it->type == empty) {
                UpdateNode(it->pos);
                it = activeAnimations.erase(it);
                continue;
            }
            if (it->type == wall && it->iter >= wallAnim.size()) {
                UpdateNode(it->pos);
                it = activeAnimations.erase(it);
                continue;
            }
            if (it->type == start && it->iter >= startAnim.size()) {
                UpdateNode(it->pos);
                it = activeAnimations.erase(it);
                continue;
            }
            if (it->type == finish && it->iter >= finishAnim.size()) {
                UpdateNode(it->pos);
                it = activeAnimations.erase(it);
                continue;
            }
            if (it->type == visited && it->iter >= visitedAnim.size()) {
                UpdateNode(it->pos);
                it = activeAnimations.erase(it);
                continue;
            }
            if (it->type == path && it->iter >= pathAnim.size()) {
                UpdateNode(it->pos);
                it = activeAnimations.erase(it);
                continue;
            }

            it++;
        }

        // show next frame of each active animation.
        it = activeAnimations.begin();
        while (it != activeAnimations.end()) {
            QPoint gpos(mapToGlobal(it->pos));
            QRect cellRect(gpos, QSize(CELL_SIZE, CELL_SIZE));

            if (it->type != vm->at(it->pos)) {
                it->type = vm->at(it->pos);
                it->iter = 0;

                if (it->type == empty) {
                    painter->fillRect(cellRect, QBrush(Qt::white));
                    painter->drawPoint(gpos.x() + CELL_SIZE / 2, gpos.y() + CELL_SIZE / 2);
                } else if (vm->isInstantUpdateMode) {
                    if (it->type == visited || it->type == path) {
                        it->type = empty;
                    }
                }
            }

            if (it->type == wall) {
                painter->drawImage(cellRect, wallAnim[it->iter]);
            } else if (it->type == visited) {
                painter->drawImage(cellRect, visitedAnim[it->iter]);
            } else if (it->type == path) {
                painter->drawImage(cellRect, pathAnim[it->iter]);
            } else if (it->type == start) {
                painter->drawImage(cellRect, startAnim[it->iter]);
            } else if (it->type == finish) {
                painter->drawImage(cellRect, finishAnim[it->iter]);
            }

            it->iter++;
            it++;
        }
//        qDebug() << "size = " << currentAnimations.size();
        mutex.unlock();

        if (vm->IsLocked())
            QThread::msleep(ulong(interval));
        else
            QThread::msleep(10);
    }
}

void GraphicsMap::DrawMap()
{
    const QVector<QVector<NODE>> map = vm->getMap();

    mutex.lock();
    for (int x = 0; x < X_NODES; x++) {
        for (int y = 0; y < Y_NODES; y++) {
            UpdateNode(QPoint(x, y));
        }
    }
    mutex.unlock();
}

void GraphicsMap::UpdateNode(QPoint pos)
{
    NODE node = vm->at(pos);

    QPoint gpos(mapToGlobal(pos));
    QRect cellRect(gpos, QSize(CELL_SIZE, CELL_SIZE));

    QPen blackPen(QColor(150,150,150));
    blackPen.setWidth(int(CELL_SIZE * 0.2));
    painter->setPen(blackPen);

    if (node == empty) {
        painter->fillRect(cellRect, QBrush(Qt::white));
        painter->drawPoint(gpos.x() + CELL_SIZE / 2, gpos.y() + CELL_SIZE / 2);
    } else if (node == wall) {
        painter->drawImage(cellRect, wallAnim.last());
    } else if (node == visited) {
        painter->drawImage(cellRect, visitedAnim.last());
    } else if (node == path) {
        painter->drawImage(cellRect, pathAnim.last());
    } else if (node == start) {
        painter->drawImage(cellRect, startAnim.last());
    } else if (node == finish) {
        painter->drawImage(cellRect, finishAnim.last());
    }
}

void GraphicsMap::AddNode(QPoint pos, NODE node)
{
    if (node == empty) {
        QPoint gpos(mapToGlobal(pos));
        QRect cellRect(gpos, QSize(CELL_SIZE, CELL_SIZE));
        painter->fillRect(cellRect, QBrush(Qt::white));
        painter->drawPoint(gpos.x() + CELL_SIZE / 2, gpos.y() + CELL_SIZE / 2);
        return;
    }

    mutex.lock();
    activeAnimations.append(AnimationInfo(pos, node));
    mutex.unlock();
}

QPoint GraphicsMap::mapFromGlobal(QPoint pos)
{
    int x = (pos.x() - pos.x() % CELL_SIZE) / CELL_SIZE;
    int y = (pos.y() - pos.y() % CELL_SIZE) / CELL_SIZE;
    return  QPoint(x, y);
}

QPoint GraphicsMap::mapToGlobal(QPoint pos)
{
    return pos * CELL_SIZE;
}
