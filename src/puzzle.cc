#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "puzzle.h"
#include "constants.h"
#include <sstream>
#include <cstring>

ThePuzzle::ThePuzzle()
{
    width = 0;
    height = 0;
    numPairs = 0;
    in = nullptr;
}


// constructor
ThePuzzle::ThePuzzle(u_int16_t w, u_int16_t h, 
    std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
                          std::pair<u_int16_t,u_int16_t>>> n) : 
                          width(w), height(h), numberPairs(n) 
{
    width = w;
    height = h;
    numberPairs = n;

    // create the grid of cells
    createGrid();

    // assign the numbers to the cells
    size_t counter = 0;
    for (const auto& p : numberPairs) {
        counter++;
        u_int16_t x1 = p.first.first;
        u_int16_t y1 = p.first.second;
        u_int16_t x2 = p.second.first;
        u_int16_t y2 = p.second.second;
        Cell* assigner1 = findCell(x1, y1);
        Cell* assigner2 = findCell(x2, y2);
        
        assigner1->number = counter;
        assigner2->number = counter;

        // mark the cells as fixed
        assigner1->setFixed();
        assigner2->setFixed();
    }
    numPairs = counter;
}//ThePuzzle

// destructor
ThePuzzle::~ThePuzzle()
{
    deletePuzzle();
}//~ThePuzzle

void ThePuzzle::createGrid()
{
    in = new Cell(0,0); // create the first cell;
    Cell* Hconnector = in;
    Cell* Vconnector = in;
    Cell* HVconnector = in;
    for (size_t y = 0; y < height; y++) {
        size_t x = 0;
        if (y != 0) {
            HVconnector = Vconnector;

            // Create a cell downwards
            Vconnector->adjacent[DOWN] = new Cell(x,y);
            Vconnector->lines[DOWN] = new Line();
            Vconnector->lines[DOWN]->to[DOWN] = Vconnector->adjacent[DOWN];

            // Connect it with the cell upwards
            Vconnector->adjacent[DOWN]->adjacent[UP] = Vconnector;
            Vconnector->adjacent[DOWN]->lines[UP] = Vconnector->lines[DOWN];
            Vconnector->adjacent[DOWN]->lines[UP]->to[UP] = Vconnector;

            // Move help pointers down
            Vconnector = Vconnector->adjacent[DOWN];
            Hconnector = Vconnector;
        }
        for (; x < width; x++) {
            if (x == 0) continue; // skip the first cell

            // Create a cell to the right
            Hconnector->adjacent[RIGHT] = new Cell(x,y);
            Hconnector->lines[RIGHT] = new Line();
            Hconnector->lines[RIGHT]->to[RIGHT] = Hconnector->adjacent[RIGHT];

            // Connect it with the cell to the left
            Hconnector->adjacent[RIGHT]->adjacent[LEFT] = Hconnector;
            Hconnector->adjacent[RIGHT]->lines[LEFT] = Hconnector->lines[RIGHT];
            Hconnector->lines[RIGHT]->to[LEFT] = Hconnector;

            // Move helper pointer to the right
            Hconnector = Hconnector->adjacent[RIGHT];

            if (y == 0) continue; // skip the first row

            // Move helper pointer to the right
            HVconnector = HVconnector->adjacent[RIGHT];

            // Connect cells up and down with eachother
            Hconnector->adjacent[UP] = HVconnector;
            HVconnector->adjacent[DOWN] = Hconnector;
            Hconnector->lines[UP] = new Line();
            HVconnector->lines[DOWN] = Hconnector->lines[UP];
            Hconnector->lines[UP]->to[UP] = HVconnector;
            Hconnector->lines[UP]->to[DOWN] = Hconnector;
        } // for x  
    } // for y
} // ThePuzzle::createGrid

char ThePuzzle::getChar(u_int16_t number)
{
    // return the number in char
    if (number <= 9) return number + '0';

    // number has to be lesser than every digit (1-9) - '0' + every letter (A-Z)
    else if (number <= 35)
        // return capitalized alphabet 
        return number - 10 + 'A';
    std::cerr << "ERROR: Number too big" << std::endl;
    return number;
}

void ThePuzzle::printPuzzle()
{
    Cell* Hprinter = in;
    Cell* Vprinter = in;
    // Print the grid

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++) {
            if (i > 0 && Hprinter->lines[UP]->connected)
                std::cout << "+X";
            else std::cout << "+-";
            Hprinter = Hprinter->adjacent[RIGHT];
        }
        Hprinter = Vprinter;
        std::cout << "+" << std::endl;
        for (size_t j = 0; j < width; j++) {
            if (j > 0 && Hprinter->lines[LEFT]->connected)
                std::cout << "X" << Hprinter->number;
            else std::cout << "|" << Hprinter->number;
            Hprinter = Hprinter->adjacent[RIGHT];
        }
        std::cout << "|" << std::endl;
        Vprinter = Vprinter->adjacent[DOWN];
        Hprinter = Vprinter;
    }
    for (size_t j = 0; j < width; j++) {
        std::cout << "+-";
    }
    std::cout << "+" << std::endl;
}

void ThePuzzle::deletePuzzle()
{
    // delete all cells and liens
    Cell* Vdeletor = in;
    Cell* Hdeletor = Vdeletor->adjacent[RIGHT];
    for (size_t i = 0; i < height; i++) {
        if (i != 0) {
            delete Vdeletor->lines[RIGHT];
            Vdeletor = Vdeletor->adjacent[DOWN];
            delete Vdeletor->adjacent[UP];
            delete Vdeletor->lines[UP];
            Hdeletor = Vdeletor->adjacent[RIGHT];
        }
        for (size_t j = 2; j <= width; j++) {
            if (Hdeletor->lines[UP] != nullptr)
                delete Hdeletor->lines[UP];
            if (j == width) { // last cell in the row
                delete Hdeletor;
                continue;
            }
            Hdeletor = Hdeletor->adjacent[RIGHT];
            delete Hdeletor->adjacent[LEFT];
            delete Hdeletor->lines[LEFT];
        }//for j  
    }//for i
    delete Vdeletor;    
}

bool ThePuzzle::isSolved()
{
    for (u_int16_t i = 0; i < height; i++) 
        for (u_int16_t j = 0; j < width; j++) {            
            Cell* checker = findCell(j,i);
            if (checker->number == 0)
                return false; // found an empty cell}
        }
    return true; // no empty cells found
}

Line::Line()
{
    connected = false;
    for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++)
        to[dir] = nullptr;
}

Cell* ThePuzzle::findCell(u_int16_t x_coord, u_int16_t y_coord)
{
    if (x_coord >= width || y_coord >= height) {
        return nullptr; // return nullptr if out of bounds
    }

    Cell* finder = in;
    for (size_t i = 0; i < y_coord; i++) {
        finder = finder->adjacent[DOWN];
    }
    for (size_t j = 0; j < x_coord; j++) {
        finder = finder->adjacent[RIGHT];
    }

    return finder; // return the found cell
}

// void ThePuzzle::switchFinder(u_int16_t toNumber, Cell& start, Cell& end) {
//     toNumber--;
//     std::cout << toNumber + 1 << "\n";
    
//     u_int16_t x1 = numberPairs.at(toNumber).first.first; 
//     u_int16_t y1 = numberPairs.at(toNumber).first.second; 
//     u_int16_t x2 = numberPairs.at(toNumber).second.first; 
//     u_int16_t y2 = numberPairs.at(toNumber).second.second; 
//     start = *findCell(x1, y1);
//     end = *findCell(x2, y2);
// }

bool dfs::solve(Cell* curr, Cell* otherPair, ThePuzzle &p, u_int16_t currPair)
{
    for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++) {
        if (curr->adjacent[dir] == nullptr) 
            continue; // skip if no adjacent cell

        if (*curr->adjacent[dir] == *otherPair) { // found my other half
            curr->number = otherPair->number;
            curr->lines[dir]->connected = true;
            
            if (p.isSolved())
                return true; // puzzle is solved 
            if(currPair == p.numPairs)
                continue; // all pairs are done but puzzle not solved, check other directions
            // switch to the next pair
            //p.switchFinder(currPair + 1, *curr, *otherPair);

            u_int16_t x1 = p.numberPairs.at(currPair).first.first; 
            u_int16_t y1 = p.numberPairs.at(currPair).first.second; 
            u_int16_t x2 = p.numberPairs.at(currPair).second.first; 
            u_int16_t y2 = p.numberPairs.at(currPair).second.second; 
            Cell* prev = curr;
            curr = p.findCell(x1, y1);
            otherPair = p.findCell(x2, y2);

            if (solve(curr, otherPair, p, currPair + 1))
                return true; // solution found

            //p.switchFinder(currPair, *curr, *otherPair);
            x2 = p.numberPairs.at(currPair - 1).second.first; 
            y2 = p.numberPairs.at(currPair - 1).second.second; 
            curr = prev;
            curr->lines[dir]->connected = false;
            otherPair = p.findCell(x2, y2);

            continue; // check other directions
        }

        if (curr->adjacent[dir]->number != 0)
            continue; // skip if not empty

        // if the adjacent cell is empty, fill it with the current number
        curr->adjacent[dir]->number = curr->number;
    
        curr->lines[dir]->connected = true;
        if (solve(curr->adjacent[dir], otherPair,p,currPair))
            return true; // solution found
        curr->lines[dir]->connected = false;
    }
    // backtrack
    if (!curr->isFixedCell())
        curr->number = 0;    
    return false; // no solution found
}

void dfs::solveWrapper(ThePuzzle& p)
{
    solve(p.findCell(p.numberPairs.front().first.first,
                     p.numberPairs.front().first.second),
          p.findCell(p.numberPairs.front().second.first,
                     p.numberPairs.front().second.second),
          p, 1);
}