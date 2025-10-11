#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "puzzle.h"
#include "constants.h"


// constructor
ThePuzzle::ThePuzzle (u_int16_t w, u_int16_t h) : width(w), height(h) 
{
    width = w;
    height = h;

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
ThePuzzle::~ThePuzzle()
{
    // delete all cells
    Cell* Vdeletor = in;
    Cell* Hdeletor = Vdeletor->adjacent[RIGHT];
    for (size_t i = 0; i < height; i++) {
        if (i != 0) {
            Vdeletor = Vdeletor->adjacent[DOWN];
            delete Vdeletor->adjacent[UP];
            Hdeletor = Vdeletor->adjacent[RIGHT];
        }
        for (size_t j = 1; j < width; j++) {
            if (j == 1) continue; // skip the first cell
            if (j == width) { // last cell in the row
                delete Hdeletor;
                continue;
            }
            Hdeletor = Hdeletor->adjacent[RIGHT];
            delete Hdeletor->adjacent[LEFT];
        }//for j  
    }//for i
    delete Vdeletor;
}//~ThePuzzle

void ThePuzzle::printPuzzle()
{
    Cell* Hprinter = in;
    Cell* Vprinter = in;
    // Print the grid

    for (size_t i = 0; i < height; i++)
    {
        std::cout << "+-";
        for (size_t j = 1; j < width; j++) {
            std::cout << "+-";
        }
        std::cout << "+" << std::endl;
        for (size_t j = 0; j < width; j++) {
            std::cout << "|" << Hprinter->getNumber();
            Hprinter = Hprinter->adjacent[RIGHT];
        }
        std::cout << "|" << std::endl;
        Vprinter = Vprinter->adjacent[DOWN];
        Hprinter = Vprinter;
    }
    std::cout << "+-";
    for (size_t j = 1; j < width; j++) {
        std::cout << "+-";
    }
    std::cout << "+" << std::endl;
}

Cell::Cell(u_int16_t x_coord, u_int16_t y_coord) : x(x_coord), y(y_coord), 
                                                number(0), isFixed(false) 
{
    for (size_t i = 0; i < MAX_DIRECTIONS; i++) {
        adjacent[i] = nullptr;
    }
    x = x_coord;
    y = y_coord;
    number = 0; // initially empty
}//Cell