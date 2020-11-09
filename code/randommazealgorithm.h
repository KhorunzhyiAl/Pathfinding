#ifndef RANDOMMAZEALGORITHM_H
#define RANDOMMAZEALGORITHM_H


#include "mazegenerationalgorithm.h"

class RandomMazeAlgorithm : public MazeGenerationAlgorithm
{
public:
    RandomMazeAlgorithm(VirtualMap *vm);
    ~RandomMazeAlgorithm() override;

private:
    void step() override;

    int x = 0, y = 0;
};

#endif // RANDOMMAZEALGORITHM_H
