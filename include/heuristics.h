#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "puzzle.h"

class heuristics
{
    public:
        heuristics();
        ~heuristics();

        /**
         * @brief Checks if the puzzle is solvable using the heuristics
         * 
         * @return true the puzzle can still be solvable
         * @return false the puzzle is not solvable
         */
        bool isSolvable();

        /**
         * @brief Set the Puzzle object
         * 
         * @param puzzle the to be set puzzle object
         */
        void setPuzzle(ThePuzzle* puzzle);
    private:
        /**
         * @brief Checks the corners of the puzzle whether one of them is enclosed
         * 
         * @return true one of the corners are enclosed
         * @return false none of the corners are enclosed
         */
        bool checkCorners();
        ThePuzzle* p;
};

#endif