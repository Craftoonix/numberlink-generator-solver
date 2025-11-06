#include "kruskal.h"
#include <algorithm>
#include <random>

kruskal::kruskal()
{

}

kruskal::~kruskal()
{

}

void kruskal::randomizeEdges(ThePuzzle p)
{
    // store lines
    Cell* Hassigner = p.in;
    Cell* Vassigner = p.in;
    while (Vassigner != nullptr) 
    {    
        while (Hassigner != nullptr) 
        {
            if (Hassigner->adjacent[RIGHT] != nullptr)
                randomizedLines.push_back(Hassigner->lines[RIGHT]);
            if (Hassigner->adjacent[UP] != nullptr)
                randomizedLines.push_back(Hassigner->lines[UP]);
            Hassigner = Hassigner->adjacent[RIGHT];
        }
        Vassigner = Vassigner->adjacent[DOWN];
        Hassigner = Vassigner;
    }   
    //shuffle vector
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(randomizedLines), std::end(randomizedLines), rng);
}

void kruskal::solve(ThePuzzle p)
{
    Line* curr = nullptr;
    randomizeEdges(p);


    curr = randomizedLines.back();
    randomizedLines.pop_back();
    enabledLines.push_back(curr);
    std::vector<Line*> currUsedLines;

    while (!randomizedLines.empty())
    {
        for(size_t dir = 0; dir < MAX_DIRECTIONS; dir++)
        {
            if (curr->to[dir] != nullptr){
                if (isAvailable(curr,dir)) {
                    curr->connected = true;
                    curr = randomizedLines.back();
                    randomizedLines.pop_back();
                    enabledLines.push_back(curr);
                    usedLines.push_back(currUsedLines);
                    currUsedLines.clear();
                    break;
                }
                else {
                    currUsedLines.push_back(curr);
                    randomizedLines.pop_back();
                    curr = randomizedLines.back();
                    break;
                }
            }
        }
    }
}

bool kruskal::isAvailable(Line* curr, size_t dir)
{
    size_t connectedLines = 0;
    for (size_t i = 0; i < MAX_DIRECTIONS; i++)
    {   
        // check if the adjacent cell is connected
        if (curr->to[dir]->adjacent[i])  
            connectedLines++;  
    }
    if (connectedLines > 1) {
        return false;
    }
    return true;
}
