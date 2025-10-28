#include "sat.h"


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
