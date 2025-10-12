#ifndef PUZZLE_H
#define PUZZLE_H

#include "constants.h"
#include <vector>
#include <utility>

/**
 * @brief Cardinal Directions of a cell
 * 
 */
enum Direction { UP, RIGHT, DOWN, LEFT };


/**
 * @brief The Cell class represents a cell in the puzzle
 * 
 */
class Cell
{
    public:
        Cell(u_int16_t x_coord, u_int16_t y_coord);
        ~Cell() = default;
        Cell* adjacent[MAX_DIRECTIONS];      // up, right, down, left (The adjacent cells)
        
        u_int16_t number; // the number in the cell (0 if empty)

    private:
        u_int16_t x, y;  // coordinates
        bool isFixed;           // is the cell fixed (given as input)?
};       

class ThePuzzle {
    public:
        ThePuzzle(u_int16_t w, u_int16_t h, 
            std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
                                  std::pair<u_int16_t,u_int16_t>>> n);
        ~ThePuzzle ();
        
        /**
         * @brief Prints the puzzle to the console
         * 
         */
        void printPuzzle();
    
    private:
        Cell* in;      // pointer to the first cell of the puzzle
        u_int16_t width;   // and width
        u_int16_t height;  // actual height
        std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
                              std::pair<u_int16_t,u_int16_t>
        >> numberPairs; // vector of pairs of coordinates of numbers
};//ThePuzzle



#endif // PUZZLE_H