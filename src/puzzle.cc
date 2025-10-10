#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "puzzle.h"
#include "constants.h"


// constructor
ThePuzzle::ThePuzzle (u_int8_t w, u_int8_t h) : width(w), height(h) {

    width = w;
    height = h;

    if ( width > MAX_CELLS || height > MAX_CELLS ) {
        std::cerr << "Error: maximum width & height is " << MAX_CELLS << std::endl;
            exit (EXIT_FAILURE);
    }//if
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            board[i][j] = 0;
        }//for j
    }//for i
}//ThePuzzle

// destructor
ThePuzzle::~ThePuzzle ( ) {
    // nothing to do here
}//~ThePuzzle