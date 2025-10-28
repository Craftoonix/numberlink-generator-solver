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

    // Assign literals for SAT
    size_t index = 0;
    for (u_int16_t y = 0; y < height ; y++)
    {
        for (u_int16_t x = 0; x < width; x++)
        {
            for (u_int16_t c = 1; c < numPairs + 1; c++)
            {
                index++;
                lit.c.push_back(std::make_tuple(x,y,c,index + totalLines));
            }
        }
    }
    lit.totalLiterals = index + totalLines;
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
    // assign literal
    totalLines++;
    addedCell->line[dir] = totalLines;
    // vertical line literal = true if cell at (x,y) is connected to cell at (x,y+1)
    // horizontal line literal = true if cell at (x,y) is connected to cell at (x+1,y)
    if (dir == DOWN)
        lit.v.push_back(std::make_tuple(addedCell->x,addedCell->y-1,totalLines));
    if (dir == RIGHT)
        lit.h.push_back(std::make_tuple(addedCell->x-1,addedCell->y,totalLines));
    if (dir == UP)
        lit.v.push_back(std::make_tuple(addedCell->x,addedCell->y,totalLines));
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

bool sat::decode(ThePuzzle &p)
{
    std::ifstream file("output.txt");
    std::string line;
    std::stringstream literals;
    int literal;

    // get the first line to determine satisfiability
    getline(file,line);
    if (strcmp(line.c_str(),"UNSAT")==0)
        return false;

    // every line after contains what literal is true
    while (getline(file,line))
    {  
        literals << line;   
    }

    // parse line to individual vars
    while (literals >> literal)
    {
        // ignore negatives and end of line 0's
        if (literal <= static_cast<int>(p.getNumEdges()))
            continue;
        
        // look up coordinate and number
        std::tuple<u_int16_t,u_int16_t,u_int16_t> 
            tup = getCoordinate(p, literal);
        Cell* curr = p.findCell(std::get<0>(tup),std::get<1>(tup));

        // assign the number to its cell
        curr->number = std::get<2>(tup);

    }
    file.close();
    return true;
}

void sat::solve(ThePuzzle& p, u_int8_t width, u_int8_t height)
{
    generateCNF(p,width,height); //convert puzzle to CNF

    // call minisat to determine what literals satisfy the puzzle
    system("/usr/bin/minisat -verb=0 numberlink.cnf output.txt");
    
    decode(p);  // decode the literals
}

void sat::generateCNF(ThePuzzle& p, u_int8_t width, u_int8_t height)
{
    nLiterals = p.lit.totalLiterals;
    nClauses = 0;
    std::ofstream file("numberlink.cnf");
    std::ostringstream cnf;
    if (!file.is_open()){
        return;
    }

    // go through each cell
    for (u_int16_t x = 0; x < width; x++) {
        for (u_int16_t y = 0; y < height; y++) {
            Cell* current = p.findCell(x,y);

            // Gather all line literals
            std::vector<u_int16_t> lineLiterals;
            for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++)
            {
                if (current->line[dir] == 0) continue;
                lineLiterals.push_back(current->line[dir]);
            }

            // Determine the amount of lines connected to cell
            u_int16_t nLines = 0;
            for (size_t dir = 0; dir < MAX_DIRECTIONS; dir++)
                if (current->adjacent[dir] != nullptr)
                    nLines++;


            // Every nu in/out.mber cell has only 1 line going in/out, every non-number cell
            // has 2 lines going Denoting corresponding logic in CNF.
            // (1 or 2 True out of 2, 3 or 4 literals).
            if (current->number > 0) {           
                switch (nLines)
                {
                case 2: // 1 true out of 2 lines
                    cnf << lineLiterals[0] << " " << lineLiterals[1] << " " << 0 << std::endl;
                    cnf << -lineLiterals[0] << " " << -lineLiterals[1] << " " << 0 << std::endl;
                    nClauses += 2;
                    //commitLiterals(lineLiterals, cnf, true, true);
                    break;
                case 3: // 1 true out of 3 lines
                    doCombinations(lineLiterals, 2, cnf, true, false);
                    commitLiterals(lineLiterals, cnf, false, true);
                    break;
                case 4: // 1 true out of 4 lines
                    doCombinations(lineLiterals, 2, cnf, true, false);
                    commitLiterals(lineLiterals, cnf, false, true);
                    break;
                } 
            } // if number
            else {
                switch (nLines)
                {
                case 2: // 2 true out of 2 lines
                    cnf << lineLiterals[0] << " " << 0 << std::endl;
                    cnf << lineLiterals[1] << " " << 0 << std::endl;
                    //commitLiterals(lineLiterals, cnf, false, true);
                    break;
                case 3: // 2 true out of 3 lines
                    doCombinations(lineLiterals, 2, cnf, false, true);
                    commitLiterals(lineLiterals, cnf, true, false);
                    break;
                case 4: // 2 true out of 4 lines
                    doCombinations(lineLiterals, 3, cnf, false, true);    
                    doCombinations(lineLiterals, 3, cnf, true, false);    
                    break;
                } // switch linesConnected
            } // else

            // Number variables for current cell. Only 1 is True. We essentially create a
            // CNF XOR-gate.
            std::vector<u_int16_t> colorVars;
            for (u_int16_t c = 1; c <= p.numPairs; c++)
                colorVars.push_back(findLiteral(p,x,y,c));
            doCombinations(colorVars, 2, cnf, true, false);
            commitLiterals(colorVars, cnf, false, true);

            // Vertical/horizontal line implies number shared with adjacent cell.
            std::vector<std::vector<u_int16_t>> currColors;
            if (current->line[RIGHT] != 0)
            {
                for (size_t c = 1; c <= p.numPairs; c++)
                {
                    std::vector<u_int16_t> h = {findLiteral(p,x,y,c), findLiteral(p,x+1,y,c)};
                    currColors.push_back(h);
                }
                for (auto product : products(currColors))
                {
                    cnf << -current->line[RIGHT] << " ";
                    commitLiterals(product,cnf,false,true);
                }
            }
            currColors.clear();
            if (current->line[DOWN] != 0)
            {
                for (size_t c = 1; c <= p.numPairs; c++)
                {
                    std::vector<u_int16_t> v = {findLiteral(p,x,y,c), findLiteral(p,x,y+1,c)};
                    currColors.push_back(v);
                }
                for (auto product : products(currColors))
                {
                    cnf << -current->line[DOWN] << " ";
                    commitLiterals(product, cnf, false, true);
                }
            }  
        } // for j
    } // for i

    // Add respective numbers to given number locations.
    for (size_t c = 1; c <= p.numPairs; c++)
    {
        cnf << findLiteral(p,p.numberPairs.at(c-1).first.first, p.numberPairs.at(c-1).first.second,c) << " " << 0 << std::endl;
        cnf << findLiteral(p,p.numberPairs.at(c-1).second.first, p.numberPairs.at(c-1).second.second,c) << " " << 0 << std::endl;
        nClauses += 2;
    }

    file << "p cnf " << nLiterals << " " << nClauses << std::endl;
    file << cnf.str();
} // generateCNF

u_int16_t sat::findLiteral(ThePuzzle& p, u_int16_t x, u_int16_t y, u_int16_t c)
{
    for (auto tup : p.lit.c)
        if (std::get<0>(tup) == x && std::get<1>(tup) == y && std::get<2>(tup) == c)
            return std::get<3>(tup);
    exit(EXIT_FAILURE);
}

std::tuple<u_int16_t,u_int16_t,u_int16_t> sat::getCoordinate(ThePuzzle& p, u_int16_t literal)
{
    for (auto tup : p.lit.c)
        if (std::get<3>(tup) == literal)
            return std::make_tuple(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup));
    exit(EXIT_FAILURE);
}

void sat::doCombinations(std::vector<u_int16_t> v, u_int16_t r, std::ostringstream & cnf, bool sign, bool unsign)
{
    std::vector<std::vector<u_int16_t>> comboVector = combinations(v,r);
    for (auto & combination: comboVector)
        commitLiterals(combination, cnf, sign, unsign);
}

void sat::commitLiterals(std::vector<u_int16_t> v, std::ostringstream & cnf, bool sign, bool unsign)
{
    for (u_int16_t literal : v)
    {
        if (sign)   cnf << -literal << " ";
        if (unsign) cnf << literal  << " ";
    }
    cnf << 0 << std::endl;
    nClauses++;

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
