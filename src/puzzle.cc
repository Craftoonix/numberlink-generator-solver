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
            u_int16_t up, down;
            if (y % 2) {
                up   = (int)Direction::LEFTUP;
                down = (int)Direction::RIGHTDOWN;
            }
            else {
                up   = (int)Direction::RIGHTUP;
                down = (int)Direction::LEFTDOWN;
            }

            // Create a cell downwards
            Vconnector->adjacent[down] = new Cell(x,y);
            Vconnector->lines[down] = new Line();
            Vconnector->lines[down]->to[down] = Vconnector->adjacent[down];

            // Connect it with the cell upwards
            Vconnector->adjacent[down]->adjacent[up] = Vconnector;
            Vconnector->adjacent[down]->lines[up] = Vconnector->lines[down];
            Vconnector->adjacent[down]->lines[up]->to[up] = Vconnector;

            // Move help pointers down
            Vconnector = Vconnector->adjacent[down];
            Hconnector = Vconnector;
        }
        for (; x < width; x++) {
            if (x == 0) continue; // skip the first cell

            // Create a cell to the (int)Direction::RIGHT
            Hconnector->adjacent[(int)Direction::RIGHT] = new Cell(x,y);
            Hconnector->lines[(int)Direction::RIGHT] = new Line();
            Hconnector->lines[(int)Direction::RIGHT]->to[(int)Direction::RIGHT] = Hconnector->adjacent[(int)Direction::RIGHT];

            // Connect it with the cell to the (int)Direction::LEFT
            Hconnector->adjacent[(int)Direction::RIGHT]->adjacent[(int)Direction::LEFT] = Hconnector;
            Hconnector->adjacent[(int)Direction::RIGHT]->lines[(int)Direction::LEFT] = Hconnector->lines[(int)Direction::RIGHT];
            Hconnector->lines[(int)Direction::RIGHT]->to[(int)Direction::LEFT] = Hconnector;

            // Move helper pointer to the (int)Direction::RIGHT
            Hconnector = Hconnector->adjacent[(int)Direction::RIGHT];

            if (y == 0) continue; // skip the first row

            // Move helper pointer to the (int)Direction::RIGHT
            HVconnector = HVconnector->adjacent[(int)Direction::RIGHT];

            // Connect cells up and down with eachother
            if (y % 2)
            {
                Hconnector->adjacent[(int)Direction::LEFTUP] = HVconnector;
                HVconnector->adjacent[(int)Direction::RIGHTDOWN] = Hconnector;
                Hconnector->lines[(int)Direction::LEFTUP] = new Line();
                HVconnector->lines[(int)Direction::RIGHTDOWN] = Hconnector->lines[(int)Direction::LEFTUP];
                Hconnector->lines[(int)Direction::LEFTUP]->to[(int)Direction::LEFTUP] = HVconnector;
                Hconnector->lines[(int)Direction::LEFTUP]->to[(int)Direction::RIGHTDOWN] = Hconnector;
                HVconnector->adjacent[(int)Direction::RIGHTDOWN] = Hconnector->adjacent[(int)Direction::LEFT];
                Hconnector->adjacent[(int)Direction::LEFT]->adjacent[(int)Direction::RIGHTUP] = HVconnector;
                HVconnector->lines[(int)Direction::LEFTDOWN] = new Line();
                HVconnector->lines[(int)Direction::LEFTDOWN]->to[(int)Direction::RIGHTUP] = HVconnector; 
                HVconnector->lines[(int)Direction::LEFTDOWN]->to[(int)Direction::LEFTDOWN] = Hconnector->adjacent[(int)Direction::LEFT];
                Hconnector->adjacent[(int)Direction::LEFT]->lines[(int)Direction::RIGHTUP] = HVconnector->lines[(int)Direction::LEFTDOWN];
            }
            else {
                Hconnector->adjacent[(int)Direction::RIGHTUP] = HVconnector;
                HVconnector->adjacent[(int)Direction::LEFTDOWN] = Hconnector;
                Hconnector->lines[(int)Direction::RIGHTUP] = new Line();
                HVconnector->lines[(int)Direction::LEFTDOWN] = Hconnector->lines[(int)Direction::RIGHTUP];
                Hconnector->lines[(int)Direction::RIGHTUP]->to[(int)Direction::RIGHTUP] = HVconnector;
                Hconnector->lines[(int)Direction::RIGHTUP]->to[(int)Direction::LEFTDOWN] = Hconnector;
                HVconnector->adjacent[(int)Direction::LEFT]->adjacent[(int)Direction::RIGHTDOWN] = Hconnector;
                Hconnector->adjacent[(int)Direction::LEFTUP] = HVconnector->adjacent[(int)Direction::LEFT];
                Hconnector->lines[(int)Direction::LEFTUP] = new Line();
                Hconnector->lines[(int)Direction::LEFTUP]->to[(int)Direction::LEFTUP] = HVconnector->adjacent[(int)Direction::LEFT];
                Hconnector->lines[(int)Direction::LEFTUP]->to[(int)Direction::RIGHTDOWN] = Hconnector;
                HVconnector->adjacent[(int)Direction::LEFT]->lines[(int)Direction::RIGHTDOWN] = Hconnector->lines[(int)Direction::LEFTUP];
            }
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
    Cell* HVprinter = in;
    // Print the grid

    for (size_t j = 0; j < width; j++) {
        std::cout << " / \\";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < height; i++)
    {
        HVprinter = Hprinter;
        if (i % 2) std::cout << "  ";
        for (size_t j = 0; j < width; j++) {
            if (j > 0 && Hprinter->lines[(int)Direction::LEFT]->connected)
                std::cout << "X ";
            else {
                std::cout << "| ";
            }
            std::cout << Hprinter->number << " ";
            Hprinter = Hprinter->adjacent[(int)Direction::RIGHT];
        }
        std::cout << "|" << std::endl;

        if (i % 2) {
            if (i < (size_t)height - 1)
                std::cout << " /";
            else {std::cout << "  ";};
        }
        for (size_t j = 0; j < width; j++) {
            if (i < (size_t)height - 1) {
                if ((i % 2)){
                    if (HVprinter->lines[(int)Direction::LEFTDOWN]->connected)
                        std::cout << " X";
                    else std::cout << " \\";
                    if ((j < (size_t)width - 1) && HVprinter->lines[(int)Direction::RIGHTDOWN]->connected)
                        std::cout << " X";
                    else std::cout << " /";
                }
                else {
                    if (j > 0 && HVprinter->lines[(int)Direction::LEFTDOWN]->connected)
                    std::cout << " X";
                    else std::cout << " \\";
                    if (HVprinter->lines[(int)Direction::RIGHTDOWN]->connected)
                        std::cout << " X";
                    else std::cout << " /";
                }
            }
            else std::cout << " \\ /";
            HVprinter = HVprinter->adjacent[(int)Direction::RIGHT];
        }
        if (!(i % 2) && (i < (size_t)height - 1)) std::cout << " \\";
        std::cout << std::endl;

        if (i % 2)
            Vprinter = Vprinter->adjacent[(int)Direction::LEFTDOWN];
        else
            Vprinter = Vprinter->adjacent[(int)Direction::RIGHTDOWN];
        Hprinter = Vprinter;
            // for (size_t j = 0; j < width; j++) {
                //     if (i > 0 && Hprinter->lines[UP]->connected)
                //         std::cout << "+X";
                //     else std::cout << " / \\";
                //     Hprinter = Hprinter->adjacent[(int)Direction::RIGHT];
                // }

       
        // for (size_t j = 0; j < width; j++) {
        //     if (j > 0 && Hprinter->lines[(int)Direction::LEFT]->connected)
        //         std::cout << "X" << getChar(Hprinter->number);
        //     else std::cout << "|" << getChar(Hprinter->number);
        //     Hprinter = Hprinter->adjacent[(int)Direction::RIGHT];
        // }
        // std::cout << "|" << std::endl;
        // Hprinter = Vprinter;
    }
    // for (size_t j = 0; j < width; j++) {
    //     std::cout << "+-";
    // }
    // std::cout << "+" << std::endl;
}

void ThePuzzle::deletePuzzle()
{
    // delete all cells and liens
    Cell* Vdeletor = in;
    Cell* Hdeletor = Vdeletor->adjacent[(int)Direction::RIGHT];
    for (size_t i = 0; i < height; i++) {
        if (i != 0) {
            delete Vdeletor->lines[(int)Direction::RIGHT];
            if (i % 2) {
                Vdeletor = Vdeletor->adjacent[(int)Direction::RIGHTDOWN];
                delete Vdeletor->adjacent[(int)Direction::LEFTUP];
                delete Vdeletor->lines[(int)Direction::LEFTUP];
            }
            else{
                Vdeletor = Vdeletor->adjacent[(int)Direction::LEFTDOWN];
                delete Vdeletor->adjacent[(int)Direction::RIGHTUP]; 
            } 
            delete Vdeletor->lines[(int)Direction::RIGHTUP];
            Hdeletor = Vdeletor->adjacent[(int)Direction::RIGHT];
        }
        for (size_t j = 2; j <= width; j++) {
            if (Hdeletor->lines[(int)Direction::LEFTUP] != nullptr)
                delete Hdeletor->lines[(int)Direction::LEFTUP];
            if (Hdeletor->lines[(int)Direction::RIGHTUP] != nullptr)
                delete Hdeletor->lines[(int)Direction::RIGHTUP];
            if (j == width) { // last cell in the row
                delete Hdeletor;
                continue;
            }
            Hdeletor = Hdeletor->adjacent[(int)Direction::RIGHT];
            delete Hdeletor->adjacent[(int)Direction::LEFT];
            delete Hdeletor->lines[(int)Direction::LEFT];
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
        if (i % 2)
            finder = finder->adjacent[(int)Direction::LEFTDOWN];
        else finder = finder->adjacent[(int)Direction::RIGHTDOWN];
    }
    for (size_t j = 0; j < x_coord; j++) {
        finder = finder->adjacent[(int)Direction::RIGHT];
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
            //p.printPuzzle();
            
            if (p.isSolved()) 
            return true; // puzzle is solved 
            if(currPair == p.numPairs) {
                curr->lines[dir]->connected = false;
                continue; // all pairs are done but puzzle not solved, check other directions
            }
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