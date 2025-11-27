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
    if (!parity())   
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

bool heuristics::parity()
{
    u_int16_t totalCells = p->width * p->height;
    u_int16_t minCellsOccupied = 0; // minimal number of occupied cells
    u_int16_t availableCells = totalCells - 2*p->numPairs; // amount of empty cells

    // setup minCellsOccupied
    for (auto it : p->numberPairs)
    {
        minCellsOccupied += abs(it.first.first - it.first.second)
                          + abs(it.second.first - it.second.second);        
    }
    minCellsOccupied -= p->numPairs;

    return ((availableCells - minCellsOccupied) % 2 == totalCells % 2);
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

    return false;
}