#ifndef PUZZLE_H
#define PUZZLE_H

#include "constants.h"

class ThePuzzle {
    public:
        ThePuzzle (u_int8_t w, u_int8_t h);
        ~ThePuzzle ( );
    
    private:
        Cell* in;      // pointer to the first cell of the puzzle
        u_int8_t width;   // and width
        u_int8_t height;  // actual height
};//ThePuzzle

struct Cell
{
    u_int8_t x, y, number;  // coordinates and number
    bool isFixed;           // is the cell fixed (given as input)?
    Cell* adjacent[4];      // up, right, down, left (The adjacent cells)
};

enum Direction { UP, RIGHT, DOWN, LEFT };

#endif // PUZZLE_H