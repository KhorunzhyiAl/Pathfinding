#include "randommazealgorithm.h"

RandomMazeAlgorithm::RandomMazeAlgorithm(VirtualMap *vm)
    : MazeGenerationAlgorithm(vm)
{

}

RandomMazeAlgorithm::~RandomMazeAlgorithm()
{

}

void RandomMazeAlgorithm::step()
{
    for (; x < X_NODES; x++) {
        int r = rand() % 100;
        if (r < 30) {
            vm->addNode(QPoint(x,y), NODE::wall);
        }
    }
    x = 0;

    if (++y >= Y_NODES) {
        finishWork();
    }
}
