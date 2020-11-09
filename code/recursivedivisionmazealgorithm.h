#ifndef RECURSIVEDIVISIONMAZEALGORITHM_H
#define RECURSIVEDIVISIONMAZEALGORITHM_H

#include <QObject>
#include <QThread>
#include <QRandomGenerator>
#include <QtConcurrent/QtConcurrentRun>
#include "mazegenerationalgorithm.h"
#include "randommazealgorithm.h"
#include "virtualmap.h"

class RecursiveDivisionMazeAlgorithm : public MazeGenerationAlgorithm
{
public:
    enum Type {
        VERTICAL,
        HORIZONTAL,
        NORMAL
    };

    RecursiveDivisionMazeAlgorithm(VirtualMap *vm, RecursiveDivisionMazeAlgorithm::Type type);
    ~RecursiveDivisionMazeAlgorithm() override;


protected slots:
    void startInNewThread() override;

private:
    int randBetween(int a, int b);
    void recDivision(int x, int y, int w, int h);
    void drawWall();
    bool canDivide(int x1, int y1, int x2, int y2);

    Type type;
};

#endif // RECURSIVEDIVISIONMAZEALGORITHM_H
