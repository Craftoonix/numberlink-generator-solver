#ifndef PUZZLE_H
#define PUZZLE_H

#include "constants.h"

class ThePuzzle {
  public:
    ThePuzzle (u_int8_t w, u_int8_t h);
    ~ThePuzzle ( );


    
  private:
    short board[MAX_CELLS][MAX_CELLS];  // the target puzzle
    short height;                       // actual height
    short width;                        // and width
};//ThePuzzle

#endif // PUZZLE_H