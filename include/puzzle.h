#ifndef PUZZLE_H
#define PUZZLE_H

#include "constants.h"
#include <vector>
#include <utility>

/**
 * @brief Cardinal Directions of a cell
 * 
 */
enum Direction { UP, RIGHT, DOWN, LEFT };

struct literals
{
    std::vector<std::tuple<u_int16_t,u_int16_t,u_int16_t>> v;
    std::vector<std::tuple<u_int16_t,u_int16_t,u_int16_t>> h;
    std::vector<std::tuple<u_int16_t,u_int16_t,u_int16_t,u_int16_t>> c;
    u_int16_t totalLiterals;
};

/**
 * @brief The Cell class represents a cell in the puzzle
 * 
 */
class Cell
{
    public:
        Cell(u_int16_t x_coord, u_int16_t y_coord);
        ~Cell() = default;
        Cell* adjacent[MAX_DIRECTIONS];      // up, right, down, left (The adjacent cells)
        Cell* checked[MAX_DIRECTIONS];       // used for backtracking

        // Kruskal's algorithm members
        Cell* inPath;              // pointer to the cell where the path comes from
        Cell* outPath;             // pointer to the cell where the path goes to

        // SAT members
        u_int16_t line[MAX_DIRECTIONS]; // index of lines starting at 1
        
        u_int16_t number; // the number in the cell (0 if empty)
        void setFixed();
        bool isFixedCell() const;
        
        /**
         * @brief equality operator 
         * 
         * @param other the other cell to compare to
         * @return true coordinates are the same
         * @return false coordinates are different
         */
        bool operator==(const Cell& other) const;

        // for debugging
        void printCoordinates() const {
            std::cout << "(" << x << "," << y << ")(" << number << ") \n";
        }

        u_int16_t x, y;  // coordinates
        
    private:
        bool isFixed;           // is the cell fixed (given as input)?
};       

class ThePuzzle {
    private:
        u_int16_t width;   // and width
        u_int16_t height;  // actual height

        // SAT
        u_int16_t totalLines;
        //std::vector<u_int16_t> hlines;
        //std::vector<u_int16_t> vlines;
        
        public:
        ThePuzzle(u_int16_t w, u_int16_t h, 
            std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
            std::pair<u_int16_t,u_int16_t>>> n);
        ~ThePuzzle ();
            
        u_int16_t numPairs; // number of pairs of numbers
            /**
             * @brief Prints the puzzle to the console
             * 
         */
        void printPuzzle();
        bool isSolved(); // check if the puzzle is solved
        
        void switchFinder(u_int16_t toNumber, Cell& start, Cell& end);
        
        size_t getNumEdges() const {
            return ((width - 1) * height) + ((height - 1) * width);
        }
        size_t getLiterals() const { return totalLines;}
        literals lit;

        
        void increaseLine(Cell* addedCell, Direction dir);
        /**
         * @brief finds a cell given its coordinates starting from the cell "in"
         * 
         * @param x_coord the x coordinate
         * @param y_coord the y coordinate
         * @param in the starting cell
         * @return Cell* the found cell or nullptr if not found
         */
        Cell* findCell(u_int16_t x_coord, u_int16_t y_coord);

        std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
        std::pair<u_int16_t,u_int16_t>
        >> numberPairs; // vector of pairs of coordinates of numbers
        Cell* in;      // pointer to the first cell of the puzzle

        

        
};//ThePuzzle

class solver
{
    public:
        virtual bool solve(Cell* curr, Cell* otherPair, 
                            ThePuzzle &p, u_int16_t currPair) = 0;
        virtual void solveWrapper(ThePuzzle& p) = 0;
};

class dfs : public solver
{
    public:
        bool solve(Cell* curr, Cell* otherPair, ThePuzzle &p, u_int16_t currPair) override;
        void solveWrapper(ThePuzzle& p) override;
};

class kruskal : public solver
{
    public:
        bool solve(Cell* curr, Cell* otherPair, ThePuzzle &p, u_int16_t currPair) override;
        void solveWrapper(ThePuzzle& p) override;
};

class sat//, ThePuzzle
{
    private:
        u_int16_t nLiterals;
        u_int32_t nClauses;
        void generateCombinations(const std::vector<u_int16_t>& elements, u_int16_t r, 
                                    u_int16_t start, std::vector<u_int16_t>& current, 
                                        std::vector<std::vector<u_int16_t>>& result);
        std::vector<std::vector<u_int16_t>> combinations(const std::vector<u_int16_t>& elements,
                                    u_int16_t r);
        void doCombinations(std::vector<u_int16_t> v, u_int16_t r, std::ostringstream & cnf, bool sign, bool unsign);

        std::vector<std::vector<u_int16_t>> products(const std::vector<std::vector<u_int16_t>> & vectors);
        void generateProducts(const std::vector<std::vector<u_int16_t>> &vectors, u_int16_t depth, 
                    std::vector<u_int16_t>& current, std::vector<std::vector<u_int16_t>> &result);
        void commitLiterals(std::vector<u_int16_t> v, std::ostringstream & cnf, bool sign, bool unsign);
        u_int16_t findLiteral(ThePuzzle& p, u_int16_t x, u_int16_t y, u_int16_t c);
        
    public:
        //bool solve(Cell* curr, Cell* otherPair, ThePuzzle &p, u_int16_t currPair) override;
       // void solveWrapper(ThePuzzle& p, u_int16_t width, u_int16_t height);
        //void solveWrapper(ThePuzzle& p) override;
        void generateCNF(ThePuzzle& p, u_int8_t width, u_int8_t height);
};




#endif // PUZZLE_H