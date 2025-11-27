#include "heuristics.h"
#include <iostream>
#include <algorithm>

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
    if (checkBlockades())
        return false;
    return true;
}

bool heuristics::emptyNeighbor(std::vector<u_int16_t> &neighbors)
{
    for (auto it : neighbors) 
        if (it == 0) {
            //printf("empty neighbor\n"); 
            return true;
        }

    return false;
}

bool heuristics::connectingCell(std::vector<u_int16_t> &neighbors, u_int16_t num)
{
    if (num != 0) 
        for (auto it : neighbors) 
            if(it == num) {
                //printf("connecting cell\n"); 
                return true;
            }

    return false;
}

bool heuristics::connectingNeighbors(std::vector<u_int16_t> &neighbors)
{
    std::sort(neighbors.begin(),neighbors.end()); 
    if (std::adjacent_find(neighbors.begin(), neighbors.end()) 
        != neighbors.end()) {
            //printf("connecting neighbor\n"); 
            return true;
        }
    return false;
}

bool heuristics::checkBlockades()
{
    for (size_t y = 0; y < p->height; y++)
    {
        for (size_t x = 0; x < p->width; x++)
        {
            Cell* currCell = p->findCell(x,y);
            std::vector<u_int16_t> neighbors;
            for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++)
            {
                if (currCell->adjacent[dir] != nullptr)
                {
                    neighbors.push_back(currCell->adjacent[dir]->number);
                }
            }

            // for (auto it : neighbors) printf("%i ",it); 
            // printf("\n");

            // there is an empty neighboring cell, no blockade
            if (emptyNeighbor(neighbors)) continue;

            // cell can connect to neighbor, no blockade
            if (connectingCell(neighbors,currCell->number)) continue;
                
            // neighbor can connect with other neighbor, no blockade
            if (connectingNeighbors(neighbors)) continue;

            //blockade
            return true;
        }
    }


    // Cell* upLeft = p->in;
    // Cell* upRight = p->findCell(p->width - 1,0);
    // Cell* lowLeft = p->findCell(0,p->height - 1);
    // Cell* lowRight = p->findCell(p->width - 1,p->height - 1);

    // // check upper right corner
    // if (upRight->adjacent[LEFT]->number != 0 && upRight->adjacent[DOWN]->number != 0 &&
    //     upRight->adjacent[LEFT]->number != upRight->adjacent[DOWN]->number)
    //     return true;  

    // // check upper left corner
    // if (upLeft->adjacent[RIGHT]->number != 0 && upLeft->adjacent[DOWN]->number != 0 &&
    //     upLeft->adjacent[RIGHT]->number != upLeft->adjacent[DOWN]->number)
    //     return true;
    

    // // check lower right corner
    // if (lowRight->adjacent[LEFT]->number != 0 && lowRight->adjacent[UP]->number != 0 &&
    //     lowRight->adjacent[LEFT]->number != lowRight->adjacent[UP]->number)
    //     return true;
 
    // // check lower left corner
    // if (lowLeft->adjacent[RIGHT]->number != 0 && lowLeft->adjacent[UP]->number != 0 &&
    //     lowLeft->adjacent[RIGHT]->number != lowLeft->adjacent[UP]->number)
    //     return true;
    return false;
}