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

    // create the grid of cells
    in = new Cell(0,0); // create the first cell;
    Cell* Hconnector = in;
    Cell* Vconnector = in;
    Cell* HVconnector = in;
    for (size_t i = 0; i < height; i++) {
        size_t j = 0;
        if (i != 0) {
            HVconnector = Vconnector;
            Vconnector->adjacent[DOWN] = new Cell(i,j);
            Vconnector->adjacent[DOWN]->adjacent[UP] = Vconnector;
            Vconnector = Vconnector->adjacent[DOWN];
            Hconnector = Vconnector;
        }
        for (; j < width; j++) {
            if (j == 0) continue; // skip the first cell
            Hconnector->adjacent[RIGHT] = new Cell(i,j);
            Hconnector->adjacent[RIGHT]->adjacent[LEFT] = Hconnector;
            Hconnector = Hconnector->adjacent[RIGHT];
            if (i == 0) continue; // skip the first row
            HVconnector = HVconnector->adjacent[RIGHT];
            Hconnector->adjacent[UP] = HVconnector;
            HVconnector->adjacent[DOWN] = Hconnector;
        }//for j  
    }//for i
}//ThePuzzle

// destructor
ThePuzzle::~ThePuzzle ( ) {
    // nothing to do here
}//~ThePuzzle

Cell::Cell(u_int8_t x_coord, u_int8_t y_coord) : x(x_coord), y(y_coord), number(0), isFixed(false) {
    for (size_t i = 0; i < MAX_DIRECTIONS; i++) {
        adjacent[i] = nullptr;
    }
    x = x_coord;
    y = y_coord;
}//Cell