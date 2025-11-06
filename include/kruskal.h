#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "puzzle.h"
#include <vector>
#include <string>

class kruskal
{
    private:
        std::vector<Line*> randomizedLines;         // the vector of lines in randomized order
        std::vector<std::vector<Line*>> usedLines;  // the used lines
        std::vector<Line*> enabledLines;            // the lines that are added

        /**
         * @brief check if the proposed line can be added in the proposed direction
         * 
         * @param curr the proposed line
         * @param dir the proposed direction
         * @return true the line is available
         * @return false the line is not available
         */
        bool isAvailable(Line* curr, size_t dir);

        /**
         * @brief randomized the lines in randomizedLines
         * 
         * @param p the puzzle in which the lines are in
         */
        void randomizeEdges(ThePuzzle p);
    public:
        kruskal();
        ~kruskal();
        void solve(ThePuzzle p);
};


#endif 