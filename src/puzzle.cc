#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "puzzle.h"
#include "constants.h"
#include <sstream>


// constructor
ThePuzzle::ThePuzzle(u_int16_t w, u_int16_t h, 
    std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
                          std::pair<u_int16_t,u_int16_t>>> n) : 
                          width(w), height(h), numberPairs(n) 
{
    width = w;
    height = h;
    numberPairs = n;
    totalLines = 0;

    // create the grid of cells
    in = new Cell(0,0); // create the first cell;
    Cell* Hconnector = in;
    Cell* Vconnector = in;
    Cell* HVconnector = in;
    for (size_t i = 0; i < height; i++) {
        size_t j = 0;
        if (i != 0) {
            HVconnector = Vconnector;

            // Create a cell downwards
            Vconnector->adjacent[DOWN] = new Cell(j,i);
            increaseLine(Vconnector, DOWN);

            // Connect it with the cell upwards
            Vconnector->adjacent[DOWN]->adjacent[UP] = Vconnector;
            Vconnector->adjacent[DOWN]->line[UP] = Vconnector->line[DOWN];

            // Move help pointers down
            Vconnector = Vconnector->adjacent[DOWN];
            Hconnector = Vconnector;
        }
        for (; j < width; j++) {
            if (j == 0) continue; // skip the first cell

            // Create a cell to the right
            Hconnector->adjacent[RIGHT] = new Cell(j,i);
            increaseLine(Hconnector, RIGHT);

            // Connect it with the cell to the left
            Hconnector->adjacent[RIGHT]->adjacent[LEFT] = Hconnector;
            Hconnector->adjacent[RIGHT]->line[LEFT] = Hconnector->line[RIGHT];

            // Move helper pointer to the right
            Hconnector = Hconnector->adjacent[RIGHT];

            if (i == 0) continue; // skip the first row

            // Move helper pointer to the right
            HVconnector = HVconnector->adjacent[RIGHT];

            // Connect cells up and down with eachother
            Hconnector->adjacent[UP] = HVconnector;
            HVconnector->adjacent[DOWN] = Hconnector;
            increaseLine(Hconnector, UP);
            HVconnector->line[DOWN] = Hconnector->line[UP];
        }//for j  
    }//for i

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

    size_t index = 0;
    for (u_int16_t y = 0; y < height - 1; y++)
    {
        for (u_int16_t x = 0; x < width - 1; x++)
        {
            for (u_int16_t c = 1; c < numPairs + 1; c++)
            {
                index++;
                lit.c.push_back(std::make_tuple(x,y,c,index + totalLines));
            }
        }
    }
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

void ThePuzzle::increaseLine(Cell* addedCell, Direction dir)
{
    totalLines++;
    addedCell->line[dir] = totalLines;
    if (dir == DOWN)
        lit.v.push_back(std::make_tuple(addedCell->x,addedCell->y-1,totalLines));
    if (dir == RIGHT)
        lit.h.push_back(std::make_tuple(addedCell->x-1,addedCell->y,totalLines));
    if (dir == UP)
        lit.v.push_back(std::make_tuple(addedCell->x,addedCell->y,totalLines));
    addedCell->linesConnected++;
}

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
            std::cout << "|" << Hprinter->number;
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
        line[i] = 0;
    }
    x = x_coord;
    y = y_coord;
    number = 0; // initially empty
    inPath = nullptr;
    outPath = nullptr;
    isFixed = false;
    linesConnected = 0;
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
        if (solve(curr->adjacent[dir], otherPair,p,currPair)) {
            return true; // solution found
        }
    }
    // backtrack
    if (!curr->isFixedCell())
        curr->number = 0;    
    return false; // no solution found
}

void dfs::solveWrapper(ThePuzzle& p)
{
    solve(p.findCell(p.numberPairs.at(0).first.first,
                     p.numberPairs.at(0).first.second),
          p.findCell(p.numberPairs.at(0).second.first,
                     p.numberPairs.at(0).second.second),
          p, 1);
}

void kruskal::solveWrapper(ThePuzzle& p)
{
    size_t numEdges = p.getNumEdges();
    for (size_t i = 0; i < numEdges; i++) {
        
    }

}

void sat::generateCNF(ThePuzzle& p, u_int8_t width, u_int8_t height)
{
    nliterals = p.getLiterals();
    std::vector<std::string> cnf;
    std::ofstream file("numberlink.cnf");
    if (!file.is_open()){
        return;
    }


    for (u_int16_t i = 0; i < height; i++) {
        for (u_int16_t j = 0; j < width; j++) {
            Cell* current = p.findCell(j,i);
            // // sort connected lines
            // u_int16_t connectedLines[MAX_DIRECTIONS] = {0};
            // size_t index = 0;
            // for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++)
            // {
            //     connectedLines[index] = current->line[dir];
            //     index++;
            // }

            std::vector<u_int16_t> lineLiterals;
            for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++)
            {
                if (current->line[dir] == 0) continue;
                lineLiterals.push_back(current->line[dir]);
            }

            // All vertical and horizontal lines attached to current cell.
            //std::vector<std::string> lines;
            // if (j != width - 1) lines.push_back("h."+std::to_string(j)+'.'+std::to_string(i)); // right edge
            // if (i != height - 1) lines.push_back("v."+std::to_string(j)+'.'+std::to_string(i)); // down edge
            // if (j != 0) lines.push_back("h."+std::to_string(j-1)+'.'+std::to_string(i)); // left edge
            // if (i != 0) lines.push_back("v."+std::to_string(j)+'.'+std::to_string(i-1)); // up edge

            // Every number cell has only 1 line going in/out, every non-number cell
            // has 2 lines going in/out. Denoting corresponding logic in CNF.
            // (1 or 2 True out of 2, 3 or 4 literals).
            
            if (current->number > 0) {           
                switch (current->linesConnected)
                {
                case 2:
                    commitLiterals(lineLiterals, file, true, true);
                    break;
                case 3:
                    doCombinations(lineLiterals, 2, file, true, false);
                    commitLiterals(lineLiterals, file, false, true);
                    break;
                case 4:
                    doCombinations(lineLiterals, 2, file, true, false);
                    commitLiterals(lineLiterals, file, false, true);
                    break;
                } // Switch linesConnected
                    // std::ostringstream clause;
                    // clause << lines[0] << " " << lines[1]; // assuming there are exactly 2 lines
                    // cnf.push_back(clause.str());
                    // clause.str("");
                    // clause << "-" << lines[0] << " -" << lines[1];;
                    // cnf.push_back(clause.str());
            } // if number
            else {
                switch (current->linesConnected)
                {
                case 2:
                    commitLiterals(lineLiterals, file, false, true);
                    break;
                case 3:
                    doCombinations(lineLiterals, 2, file, false, true);
                    commitLiterals(lineLiterals, file, true, false);
                    break;
                case 4:
                    doCombinations(lineLiterals, 3, file, true, true);    
                    break;
                } // switch linesConnected
            } // else

            std::vector<u_int16_t> colorVars;
            for (size_t c = 1; c <= p.numPairs; c++)
                colorVars.push_back(findLiteral(p,j,i,c));
            doCombinations(colorVars, 2, file, true, false);
            commitLiterals(colorVars, file, false, true);

            std::vector<std::vector<u_int16_t>> currColors;
            if (current->line[RIGHT] != 0)
            {
                for (size_t c = 1; c <= p.numPairs; c++)
                {
                    currColors.at(c).at(0) = findLiteral(p,j,i,c);
                    currColors.at(c).at(1) = findLiteral(p,j+1,i,c);
                }
                for (auto product : products(currColors))
                {
                    file << -current->line[RIGHT] << " ";
                    commitLiterals(product,file,false,true);
                }
            }
            if (current->line[DOWN] != 0)
            {
                for (size_t c = 0; c <= p.numPairs; c++)
                {
                    currColors.at(c).at(0) = findLiteral(p,j,i,c+1);
                    currColors.at(c).at(1) = findLiteral(p,j,i+1,c+1);
                }
                for (auto product : products(currColors))
                {
                    file << -current->line[DOWN] << " ";
                    commitLiterals(product, file, false, true);
                }
            }  
        } // for j
    } // for i
    for (size_t c = 1; c <= p.numPairs; c++)
    {
        file << findLiteral(p,p.numberPairs.at(c-1).first.first, p.numberPairs.at(c-1).first.second,c) << std::endl;
        file << findLiteral(p,p.numberPairs.at(c-1).second.first, p.numberPairs.at(c-1).second.second,c) << std::endl;
    }
} // generateCNF

u_int16_t sat::findLiteral(ThePuzzle p, u_int16_t x, u_int16_t y, u_int16_t c)
{
    for (auto tup : p.lit.c)
        if (std::get<0>(tup) == x && std::get<1>(tup) == y, std::get<2>(tup) == c)
            return std::get<3>(tup);
    return 0;
}

void sat::doCombinations(std::vector<u_int16_t> v, u_int16_t r, std::ofstream & file, bool sign, bool unsign)
{
    std::vector<std::vector<u_int16_t>> comboVector = combinations(v,r);
    for (auto & combination: comboVector)
        commitLiterals(combination, file, sign, unsign);
    
}

//void sat::doProducts(std::)

void sat::commitLiterals(std::vector<u_int16_t> v, std::ofstream & file, bool sign, bool unsign)
{
    for (u_int16_t literal : v)
    {
        if (sign)   file << -literal << " ";
        if (unsign) file << literal  << " ";
    }
    file << std::endl;

}

void sat::generateCombinations(const std::vector<u_int16_t>& elements, u_int16_t r, 
    u_int16_t start, std::vector<u_int16_t>& current, std::vector<std::vector<u_int16_t>>& result) {
    if (current.size() == r) {
        result.push_back(current);
        return;
    }
    for (size_t i = start; i < elements.size(); ++i) {
        current.push_back(elements[i]);
        generateCombinations(elements, r, i + 1, current, result);
        current.pop_back();
    }
}


std::vector<std::vector<u_int16_t>> sat::combinations(const std::vector<u_int16_t>& elements, u_int16_t r) {
    std::vector<std::vector<u_int16_t>> result;
    std::vector<u_int16_t> current;
    generateCombinations(elements, r, 0, current, result);
    return result;
}

void sat::generateProducts(const std::vector<std::vector<u_int16_t>> &vectors, u_int16_t depth, 
    std::vector<u_int16_t> &current,std::vector<std::vector<u_int16_t>> &result)
{
    if (depth == vectors.size()){
        result.push_back(current);
        return;
    }

    for (u_int16_t num: vectors[depth])
    {
        current.push_back(num);
        generateProducts(vectors, depth+1, current, result);
        current.pop_back();
    }
}

std::vector<std::vector<u_int16_t>> sat::products(const std::vector<std::vector<u_int16_t>> & vectors)
{
    std::vector<std::vector<u_int16_t>> result;
    std::vector<u_int16_t> current;
    generateProducts(vectors, 0, current, result);
    return result;
}