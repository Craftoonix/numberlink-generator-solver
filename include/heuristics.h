#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "puzzle.h"

class heuristics
{
    public:
        heuristics();
        heuristics(ThePuzzle numberlink);
        bool isSolvable();
    private:
        bool checkCorner(Cell* corner);
        ThePuzzle p;
};

#endif