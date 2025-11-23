#include "heuristics.h"
#include <iostream>

heuristics::heuristics()
{
    p = nullptr;
}

heuristics::~heuristics()
{
    p = nullptr;
}

void heuristics::setPuzzle(ThePuzzle* puzzle)
{
    p = puzzle;
}

bool heuristics::isSolvable()
{
    if (checkCorners())
        return false;
    return true;
}

bool heuristics::checkCorners()
{
    Cell* upLeft = p->in;
    Cell* upRight = p->findCell(p->width - 1,0);
    Cell* lowLeft = p->findCell(0,p->height - 1);
    Cell* lowRight = p->findCell(p->width - 1,p->height - 1);

    // check upper right corner
    if (upRight->adjacent[LEFT]->number != 0 && upRight->adjacent[DOWN]->number != 0 &&
        upRight->adjacent[LEFT]->number != upRight->adjacent[DOWN]->number)
        return true;  
   

    // check upper left corner
    if (upLeft->adjacent[RIGHT]->number != 0 && upLeft->adjacent[DOWN]->number != 0 &&
        upLeft->adjacent[RIGHT]->number != upLeft->adjacent[DOWN]->number)
        return true;
    

    // check lower right corner
    if (lowRight->adjacent[LEFT]->number != 0 && lowRight->adjacent[UP]->number != 0 &&
        lowRight->adjacent[LEFT]->number != lowRight->adjacent[UP]->number)
        return true;
 
    // check lower left corner
    if (lowLeft->adjacent[RIGHT]->number != 0 && lowLeft->adjacent[UP]->number != 0 &&
        lowLeft->adjacent[RIGHT]->number != lowLeft->adjacent[UP]->number)
        return true;
    return false;
}