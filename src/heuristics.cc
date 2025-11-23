#include "heuristics.h"

heuristics::heuristics()
{
    
}

heuristics::heuristics(ThePuzzle numberlink)
{
    p = numberlink;
}

bool heuristics::isSolvable()
{
    if (checkCorner(p.in) || checkCorner(p.findCell(p.width-1,0)) ||
        checkCorner(p.findCell(p.width-1,p.height-1)) ||checkCorner(p.findCell(0,p.height-1)))
        return false;
    


    return true;
}

bool heuristics::checkCorner(Cell* corner)
{
    if(corner->adjacent[RIGHT]->number != 0 && corner->adjacent[DOWN]->number != 0)
        return true;    
    return false;
}