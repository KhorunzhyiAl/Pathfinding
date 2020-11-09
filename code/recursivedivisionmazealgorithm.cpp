#include "recursivedivisionmazealgorithm.h"

RecursiveDivisionMazeAlgorithm::RecursiveDivisionMazeAlgorithm(VirtualMap *vm, RecursiveDivisionMazeAlgorithm::Type type)
    : MazeGenerationAlgorithm(vm, true), type(type)
{

}

RecursiveDivisionMazeAlgorithm::~RecursiveDivisionMazeAlgorithm()
{

}

void RecursiveDivisionMazeAlgorithm::startInNewThread()
{
    qsrand(uint(time(nullptr)));

    drawWall();
    recDivision(1, 1, X_NODES - 2, Y_NODES - 2);

    finishWork();
}

void RecursiveDivisionMazeAlgorithm::recDivision(int x1, int y1, int x2, int y2)
{
//    qDebug() << "x1=" << x1 << "\ty1=" << y1 << "\tx2=" << x2 << "\ty2=" << y2 << "=======================================";
    if (!isWorking) return;

    bool vertical = (x2 - x1) > (y2 - y1);
    if (type == VERTICAL) vertical = (x2 - x1) * 3 > (y2 - y1);
    else if (type == HORIZONTAL) vertical = (x2 - x1) > (y2 - y1) * 3;


    if (vertical) {
        if (x2 - x1 <= 1 || y2 - y1 <= 0) return;

        int randx = randBetween(x1 + 1, x2);
        int randy = randBetween(y1, y2 + 1);

        if (!canDivide(randx, y1, randx, y2)) {
            if (randx - 1 > x1 && canDivide(randx-1, y1, randx-1, y2)) {
                randx--;
            } else if (randx + 1 < x2 && canDivide(randx+1, y1, randx+1, y2)) {
                randx++;
            } else if (randx - 2 > x1 && canDivide(randx-2, y1, randx-2, y2)) {
                randx -= 2;
            } else if (randx + 2 < x2 && canDivide(randx+2, y1, randx+2, y2)) {
                randx += 2;
            } else {
                return;
            }
        }

        for (int i = y1; i <= y2; i++) {
            if (i == randy) continue;

            QThread::msleep(uint(msDelay));
            vm->addNode(QPoint(randx, i), NODE::wall);
        }

        recDivision(randx + 1, y1, x2, y2);
        recDivision(x1, y1, randx - 1, y2);
    } else {
        if (y2 - y1 <= 1 || x2 - x1 <= 0) return;

        int randx = randBetween(x1, x2 + 1);
        int randy = randBetween(y1 + 1, y2);

        if (!canDivide(x1, randy, x2, randy)) {
            if (randy - 1 > y1 && canDivide(x1, randy-1, x2, randy-1)) {
                randy--;
            } else if (randy + 1 < y2 && canDivide(x1, randy+1, x2, randy+1)) {
                randy++;
            } else if (randy - 2 > y1 && canDivide(x1, randy-2, x2, randy-2)) {
                randy -= 2;
            } else if (randy + 2 < y2 && canDivide(x1, randy+2, x2, randy+2)) {
                randy += 2;
            } else {
                return;
            }
        }

        for (int i = x1; i <= x2; i++) {
            if (i == randx) continue;

            QThread::msleep(uint(msDelay));
            if (!isWorking) return;
            vm->addNode(QPoint(i, randy), NODE::wall);
        }

        recDivision(x1, randy + 1, x2, y2);
        recDivision(x1, y1, x2, randy - 1);
    }
}

void RecursiveDivisionMazeAlgorithm::drawWall()
{
    for (int x = 0; x < X_NODES; x++) {
        vm->addNode(QPoint(x, 0), NODE::wall);
        QThread::msleep(2);
    }

    for (int y = 1; y < Y_NODES; y++) {
        vm->addNode(QPoint(X_NODES - 1, y), NODE::wall);
        QThread::msleep(2);
    }

    for (int x = X_NODES - 2; x > 0; x--) {
        vm->addNode(QPoint(x, Y_NODES - 1), NODE::wall);
        QThread::msleep(2);
    }

    for (int y = Y_NODES - 1; y > 0; y--) {
        vm->addNode(QPoint(0, y), NODE::wall);
        QThread::msleep(2);
    }
}

bool RecursiveDivisionMazeAlgorithm::canDivide(int x1, int y1, int x2, int y2)
{
//    qDebug() << "x1:" << x1 << "\ty1:" << y1 << "\tx2:" << x2 << "\ty2:" << y2;
    if (x1 == x2) {
        return vm->at(QPoint(x1, y1 - 1)) == NODE::wall &&
                vm->at(QPoint(x2, y2 + 1)) == NODE::wall;
    }
    if (y1 == y2) {
        return vm->at(QPoint(x1 - 1, y1)) == NODE::wall &&
                vm->at(QPoint(x2 + 1, y2)) == NODE::wall;
    }
    return false;
}

int RecursiveDivisionMazeAlgorithm::randBetween(int a, int b)
{
    if (a >= b) return 0;
    return a + qrand() % (b - a);
}
