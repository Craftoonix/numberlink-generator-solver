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
         * @brief Checks for enclosures for each cell of the puzzle
         * 
         * @return true at least 1 cell is enclosed
         * @return false none of the cells are enclosed
         */
        bool checkBlockades();
        ThePuzzle* p;

        bool emptyNeighbor(std::vector<u_int16_t> &neighbor);
        bool connectingCell(std::vector<u_int16_t> &neighbors, u_int16_t num);
        bool connectingNeighbors(std::vector<u_int16_t> &neighbors);
};

#endif