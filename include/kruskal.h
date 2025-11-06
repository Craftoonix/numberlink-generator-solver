#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "puzzle.h"
#include <vector>
#include <string>

class kruskal
{
    private:
        std::vector<Line*> randomizedLines; 
        bool isAvailable(Line* curr, size_t dir);
        std::vector<std::vector<Line*>> usedLines;
        std::vector<Line*> enabledLines;
    public:
        kruskal();
        ~kruskal();
        void randomizeEdges(ThePuzzle p);
        void solve(ThePuzzle p);
};


#endif 