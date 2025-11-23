#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "puzzle.h"

class heuristics
{
    public:
        heuristics();
        ~heuristics();
        bool isSolvable();
        void setPuzzle(ThePuzzle* puzzle);
    private:
        bool checkCorners();
        ThePuzzle* p;
};

#endif