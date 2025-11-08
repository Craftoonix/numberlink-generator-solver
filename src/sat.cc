#include "sat.h"

void sat::assignLiterals(u_int16_t width, u_int16_t height, u_int16_t nPairs, ThePuzzle & p)
{
    // assign line literals
    Cell* assigner, *Vconnector;
    assigner = Vconnector = p.in;
    lit.totalLiterals = 0;

    while (Vconnector != nullptr)
    {
        while(assigner != nullptr)
        {

            if (assigner->adjacent[UP] != nullptr) {
                lit.totalLiterals++;
                lit.vl.push_back(std::make_tuple(assigner->x,assigner->y-1,lit.totalLiterals));
                // lit.totalLiterals++;
                // lit.vb.push_back(std::make_tuple(assigner->x,assigner->y-1,lit.totalLiterals));
            }

            if (assigner->adjacent[RIGHT] != nullptr)
            {
                lit.totalLiterals++;
                lit.hl.push_back(std::make_tuple(assigner->x,assigner->y,lit.totalLiterals));
                // lit.totalLiterals++;
                // lit.hb.push_back(std::make_tuple(assigner->x,assigner->y,lit.totalLiterals));
            }
            assigner = assigner->adjacent[RIGHT];
        }
        Vconnector = Vconnector->adjacent[DOWN];
        assigner = Vconnector;
    }
    

    // Assign color literals
    for (u_int16_t y = 0; y < height ; y++)
    {
        for (u_int16_t x = 0; x < width; x++)
        {
            for (u_int16_t c = 1; c < nPairs + 1; c++)
            {
                lit.totalLiterals++;
                lit.c.push_back(std::make_tuple(x,y,c,lit.totalLiterals));
            }
        }
    }  

    // assign border literals
    // for  (u_int16_t y = 0; y < height; y++)
    // {
    //     for (u_int16_t x = 0; x <= width; x++)
    //     {
    //         lit.totalLiterals++;
    //         lit.vb.push_back(std::make_tuple(x,y,lit.totalLiterals));
    //     }
    // }
    // for  (u_int16_t y = 0; y <= height; y++)
    // {
    //     for (u_int16_t x = 0; x < width; x++)
    //     {
    //         lit.totalLiterals++;
    //         lit.hb.push_back(std::make_tuple(x,y,lit.totalLiterals));
    //     }
    // }

    // assign vertex literals
    for (u_int16_t y = 1; y < height; y++)
    {
        for (u_int16_t x = 1; x <= width; x++)
        {
            lit.totalLiterals++;
            // every left upper corner of each cell 
            // meaning that every vertex at the border will be skipped
            lit.r.push_back(std::make_tuple(x,y,lit.totalLiterals));
        }
    }
}

bool sat::decode(ThePuzzle &p)
{
    std::ifstream file("output.txt");
    std::string line;
    std::stringstream literals;
    int literal;
    Cell* curr;

    // get the first line to determine satisfiability
    getline(file,line);
    if (strcmp(line.c_str(),"UNSAT")==0)
        return false;

    // every line after contains what literal is true
    while (getline(file,line))
        literals << line;   

    // parse line to individual vars
    while (literals >> literal)
    {
        // ignore negatives and end of line 0's
        if (literal <= 0) continue;

        // decode line literals
        if (literal <= static_cast<int>(p.getNumEdges()))
        {
            std::tuple<u_int16_t,u_int16_t,bool> coords;
            coords = getLineCoordinate(literal);
            curr = p.findCell(std::get<0>(coords),std::get<1>(coords));
            if (std::get<2>(coords))
                curr->lines[RIGHT]->connected = true;
            else curr->lines[DOWN]->connected = true;
            continue;
        }
        
        // decode color literals
        std::tuple<u_int16_t,u_int16_t,u_int16_t> 
        tup = getCoordinate(literal);
        curr = p.findCell(std::get<0>(tup),std::get<1>(tup));

        // assign the number to its cell
        curr->number = std::get<2>(tup);

    }
    file.close();
    return true;
}

void sat::solve(ThePuzzle& p, u_int8_t width, u_int8_t height, u_int8_t nPairs)
{
    assignLiterals(width, height, nPairs, p);
    generateCNF(p,width,height); //convert puzzle to CNF

    // call minisat to determine what literals satisfy the puzzle
    system("/usr/bin/minisat -verb=0 numberlink.cnf output.txt");
    
    decode(p);  // decode the literals
}

void sat::generateCNF(ThePuzzle& p, u_int8_t width, u_int8_t height)
{
    nLiterals = lit.totalLiterals;
    nClauses = 0;
    std::ofstream file("numberlink.cnf");
    std::ostringstream cnf;
    if (!file.is_open()){
        return;    // for  (u_int16_t y = 0; y < height; y++)
    // {
    //     for (u_int16_t x = 0; x <= width; x++)
    //     {
    //         lit.totalLiterals++;
    //         lit.vb.push_back(std::make_tuple(x,y,lit.totalLiterals));
    //     }
    // }
    // for  (u_int16_t y = 0; y <= height; y++)
    // {
    //     for (u_int16_t x = 0; x < width; x++)
    //     {
    //         lit.totalLiterals++;
    //         lit.hb.push_back(std::make_tuple(x,y,lit.totalLiterals));
    //     }
    // }
    }
    
    // go through each cell
    for (u_int16_t y = 0; y < height; y++) {
        for (u_int16_t x = 0; x < width; x++) {
            Cell* current = p.findCell(x,y);

            // Gather all line literals connected to current cell
            std::vector<u_int16_t> lineLiterals;
            if (x != 0)
                lineLiterals.push_back(findLineLiteral(x-1, y, true));
            if (x != (width - 1))
                lineLiterals.push_back(findLineLiteral(x, y, true));
            if (y != 0)
                lineLiterals.push_back(findLineLiteral(x, y-1, false));
            if (y != (height - 1))
                lineLiterals.push_back(findLineLiteral(x, y, false));

            // Determine the amount of lines connected to cell
            u_int16_t nLines = lineLiterals.size();

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
                colorVars.push_back(findNumberLiteral(x,y,c));
            doCombinations(colorVars, 2, cnf, true, false);
            commitLiterals(colorVars, cnf, false, true);

            // Vertical/horizontal line implies number shared with adjacent cell.
            std::vector<std::vector<u_int16_t>> currColors;
            if (x != (width - 1))
            {
                for (size_t c = 1; c <= p.numPairs; c++)
                {
                    std::vector<u_int16_t> h = {findNumberLiteral(x,y,c), findNumberLiteral(x+1,y,c)};
                    currColors.push_back(h);
                }
                for (auto product : products(currColors))
                {
                    cnf << -findLineLiteral(x,y,true) << " ";
                    commitLiterals(product,cnf,false,true);
                }
            }
            currColors.clear();
            if (y != (height - 1))
            {
                for (size_t c = 1; c <= p.numPairs; c++)
                {
                    std::vector<u_int16_t> v = {findNumberLiteral(x,y,c), findNumberLiteral(x,y+1,c)};
                    currColors.push_back(v);
                }
                for (auto product : products(currColors))
                {
                    cnf << -findLineLiteral(x,y,false) << " ";
                    commitLiterals(product, cnf, false, true);
                }
            }  

            if (x == 0 || y == 0) continue;

            // prevent 2x2 cycles
            cnf << -findLineLiteral(x,y-1,VERTICAL) << " ";
            cnf << -findLineLiteral(x-1,y,HORIZONTOAL) << " ";
            cnf << -findLineLiteral(x-1,y-1,VERTICAL) << " ";
            cnf << -findLineLiteral(x-1,y-1,HORIZONTOAL) << " ";
            cnf << 0 << std::endl;
            
            // u_int16_t vertex = findVertexLiteral(x,y);
            // if (vertex > 0) {
            //     if
            //     cnf << vertex << " ";
            // }

        } // for j
    } // for i

    // Add respective numbers to given number locations.
    for (size_t c = 1; c <= p.numPairs; c++)
    {
        cnf << findNumberLiteral(p.numberPairs.at(c-1).first.first, p.numberPairs.at(c-1).first.second,c) << " " << 0 << std::endl;
        cnf << findNumberLiteral(p.numberPairs.at(c-1).second.first, p.numberPairs.at(c-1).second.second,c) << " " << 0 << std::endl;
        nClauses += 2;
    }

    file << "p cnf " << nLiterals << " " << nClauses << std::endl;
    file << cnf.str();
} // generateCNF

u_int16_t sat::findNumberLiteral(u_int16_t x, u_int16_t y, u_int16_t c)
{
    for (auto tup : lit.c)
        if (std::get<0>(tup) == x && std::get<1>(tup) == y && std::get<2>(tup) == c)
            return std::get<3>(tup);
    exit(EXIT_FAILURE);
}

u_int16_t sat::findLineLiteral(u_int16_t x, u_int16_t y, bool horizontal)
{
    // the line is horizontal
    if (horizontal)
    {
        for (auto tup : lit.hl)
            if (std::get<0>(tup) == x && std::get<1>(tup) == y)
                return std::get<2>(tup);
        
        exit(EXIT_FAILURE);
    }

    // the line is vertical
    else
    {
        for (auto tup : lit.vl)
            if (std::get<0>(tup) == x && std::get<1>(tup) == y)
                return std::get<2>(tup);
        exit(EXIT_FAILURE); 
    }

}

u_int16_t sat::findVertexLiteral(u_int16_t x, u_int16_t y)
{
    for (auto tup : lit.r)
        if (std::get<0>(tup) == x && std::get<1>(tup) == y)
            return std::get<2>(tup);
    
    return 0;
}

std::tuple<u_int16_t,u_int16_t,u_int16_t> sat::getCoordinate(u_int16_t literal)
{
    for (auto tup : lit.c)
        if (std::get<3>(tup) == literal)
            return std::make_tuple(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup));
    exit(EXIT_FAILURE);
}

std::tuple<u_int16_t,u_int16_t,bool> sat::getLineCoordinate(u_int16_t literal)
{
    for (auto tup : lit.hl)
        if (std::get<2>(tup) == literal)
            return std::make_tuple(std::get<0>(tup),std::get<1>(tup),true);

    for (auto tup : lit.vl)
        if (std::get<2>(tup) == literal)
            return std::make_tuple(std::get<0>(tup),std::get<1>(tup),false);

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
