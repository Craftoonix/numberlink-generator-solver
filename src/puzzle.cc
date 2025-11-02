#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "puzzle.h"
#include "constants.h"
#include <sstream>
#include <cstring>


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

            // Connect it with the cell upwards
            Vconnector->adjacent[DOWN]->adjacent[UP] = Vconnector;

            // Move help pointers down
            Vconnector = Vconnector->adjacent[DOWN];
            Hconnector = Vconnector;
        }
        for (; x < width; x++) {
            if (x == 0) continue; // skip the first cell

            // Create a cell to the right
            Hconnector->adjacent[RIGHT] = new Cell(x,y);

            // Connect it with the cell to the left
            Hconnector->adjacent[RIGHT]->adjacent[LEFT] = Hconnector;

            // Move helper pointer to the right
            Hconnector = Hconnector->adjacent[RIGHT];

            if (y == 0) continue; // skip the first row

            // Move helper pointer to the right
            HVconnector = HVconnector->adjacent[RIGHT];

            // Connect cells up and down with eachother
            Hconnector->adjacent[UP] = HVconnector;
            HVconnector->adjacent[DOWN] = Hconnector;
        } // for x  
    } // for y
} // ThePuzzle::createGrid

void ThePuzzle::printPuzzle()
{
    Cell* Hprinter = in;
    Cell* Vprinter = in;
    // Print the grid

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++) {
            if ((i != 0 && Hprinter->adjacent[UP]->path == DOWN) 
                        || Hprinter->path == UP)
                std::cout << "+X";
            else std::cout << "+-";
            Hprinter = Hprinter->adjacent[RIGHT];
        }
        Hprinter = Vprinter;
        std::cout << "+" << std::endl;
        for (size_t j = 0; j < width; j++) {
            if ((j > 0 && Hprinter->adjacent[LEFT]->path == RIGHT) 
                       || Hprinter->path == LEFT)
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

Cell::Cell(u_int16_t x_coord, u_int16_t y_coord)                                          
{
    for (size_t i = 0; i < MAX_DIRECTIONS; i++) {
        adjacent[i] = nullptr;
    }
    x = x_coord;
    y = y_coord;
    number = 0; // initially empty
    inPath = nullptr;
    outPath = nullptr;
    isFixed = false;
    path = Direction::NOP;
}//Cell

bool Cell::operator==(const Cell& other) const
{
    return (x == other.x && y == other.y);
}

void Cell::setFixed()
{
    isFixed = true;
}

bool Cell::isFixedCell() const
{
    return isFixed;
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

void ThePuzzle::switchFinder(u_int16_t toNumber, Cell& start, Cell& end) {
    toNumber--;
    std::cout << toNumber + 1 << "\n";
    
    u_int16_t x1 = numberPairs.at(toNumber).first.first; 
    u_int16_t y1 = numberPairs.at(toNumber).first.second; 
    u_int16_t x2 = numberPairs.at(toNumber).second.first; 
    u_int16_t y2 = numberPairs.at(toNumber).second.second; 
    start = *findCell(x1, y1);
    end = *findCell(x2, y2);
}

bool dfs::solve(Cell* curr, Cell* otherPair, ThePuzzle &p, u_int16_t currPair)
{
    for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++) {
        if (curr->adjacent[dir] == nullptr) 
            continue; // skip if no adjacent cell

        if (*curr->adjacent[dir] == *otherPair) { // found my other half
            curr->number = otherPair->number;
            curr->path = static_cast<Direction>(dir);
            
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
            otherPair = p.findCell(x2, y2);

            continue; // check other directions
            
        }

        if (curr->adjacent[dir]->number != 0)
            continue; // skip if not empty

        // if the adjacent cell is empty, fill it with the current number
        curr->adjacent[dir]->number = curr->number;
        curr->path = static_cast<Direction>(dir);
        if (solve(curr->adjacent[dir], otherPair,p,currPair)) {
            return true; // solution found
        }
    }
    // backtrack
    if (!curr->isFixedCell())
        curr->number = 0;    
    curr->path = Direction::NOP;
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

void kruskal::solveWrapper(ThePuzzle& p)
{
    size_t numEdges = p.getNumEdges();
    for (size_t i = 0; i < numEdges; i++) {
        
    }

}