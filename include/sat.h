#ifndef SAT_H
#define SAT_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include "puzzle.h"

class sat
{
    private:
        u_int16_t nLiterals; // Amount of literals needed
        u_int32_t nClauses;  // Amount of clauses needed

        /**
         * @brief Returns an iterator of tuples, each containing a unique combination of elements
         * 
         * @param elements Initial elements vector
         * @param r The length of each combination
         * @return The vector of combinations
         */
        std::vector<std::vector<u_int16_t>> combinations(const std::vector<u_int16_t>& elements, u_int16_t r);
        
        /**
         * @brief A recursive helper function to generate combinations
         * 
         * @param elements Initial elements vector
         * @param r The length of each combination
         * @param start The Starting point 
         * @param current The current vector
         * @param result The resulting vector
         */
        void generateCombinations(const std::vector<u_int16_t>& elements, u_int16_t r, 
                                    u_int16_t start, std::vector<u_int16_t>& current, 
                                        std::vector<std::vector<u_int16_t>>& result);
                                        
        /**
         * @brief Will handle the combinations of vectors
         * 
         * @param v The vector needed to generate combinations
         * @param r The length of each combination
         * @param cnf The stringstream needed to dump the combinations in
         * @param sign Are the literals signed
         * @param unsign Are the literals not signed
         * @return * void 
         */
        void doCombinations(std::vector<u_int16_t> v, u_int16_t r, std::ostringstream & cnf, bool sign, bool unsign);

        /**
         * @brief Returns a vector each containing one element from each of the input vectors
         * 
         * @param vectors The input vectors
         * @return The product vector
         */
        std::vector<std::vector<u_int16_t>> products(const std::vector<std::vector<u_int16_t>> & vectors);

        /**
         * @brief A recursive helper function to generate products of vectors
         * 
         * @param vectors The input vectors
         * @param depth The depth of recursion
         * @param current The current vector
         * @param result The resulting vector
         */
        void generateProducts(const std::vector<std::vector<u_int16_t>> &vectors, u_int16_t depth, 
                    std::vector<u_int16_t>& current, std::vector<std::vector<u_int16_t>> &result);
        
        /**
         * @brief Will stream the literals in v to cnf
         * 
         * @param v The vector containing the literals
         * @param cnf The ostringstream needed to contain the literals 
         * @param sign Are the literals signed
         * @param unsign Are the literals not signed
         */
        void commitLiterals(std::vector<u_int16_t> v, std::ostringstream & cnf, bool sign, bool unsign);
        
        /**
         * @brief Looks up the literal given the x and y coordinate and their number
         * 
         * @param p The puzzle
         * @param x The X coordinate
         * @param y The Y coordinate
         * @param c The number associated
         * @return The literal 
         */
        u_int16_t findLiteral(ThePuzzle& p, u_int16_t x, u_int16_t y, u_int16_t c);
        
        /**
         * @brief Decodes output.txt to visually see the solution
         * 
         * @param p The Puzzle
         * @return true Puzzle is solved
         * @return false Puzzle is unsolvable
         */
        bool decode(ThePuzzle & p);

        /**
         * @brief Get coordinates and number from a given literal
         * 
         * @param p The Puzzle
         * @param literal The given literal
         * @return A tuple containing the coordinates and number
         */
        std::tuple<u_int16_t,u_int16_t,u_int16_t> getCoordinate(ThePuzzle& p, u_int16_t literal);
        
    public:
        /**
         * @brief Solves the puzzle using SAT
         * 
         * @param p The puzzle
         * @param width The width of the puzzle
         * @param height The height of the puzzle
         */
        void solve(ThePuzzle& p, u_int8_t width, u_int8_t height);
        
        /**
         * @brief Encodes the puzzle into CNF based on 
         * https://github.com/uguryavuz/numberlink-solver/blob/master/NumberlinkPuzzle.py
         * 
         * @param p The puzzle
         * @param width The width of the puzzle
         * @param height The height of the puzzle
         */
        void generateCNF(ThePuzzle& p, u_int8_t width, u_int8_t height);
};


#endif