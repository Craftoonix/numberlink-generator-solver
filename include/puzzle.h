#ifndef PUZZLE_H
#define PUZZLE_H

#include "constants.h"
#include <vector>
#include <utility>

/**
 * @brief Cardinal Directions of a cell
 * 
 */
enum Direction {UP, RIGHT, DOWN, LEFT};


/**
 * @brief Blueprint of a struct containing all literals
 * 
 */
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
        
        /**
         * @brief Set the cell as fixed
         * 
         */
        void setFixed();

        /**
         * @brief Is the cell fixed
         * 
         * @return true the cell is fixed
         * @return false the cell is not fixed
         */
        bool isFixedCell() const;
        
        /**
         * @brief equality operator 
         * 
         * @param other the other cell to compare to
         * @return true coordinates are the same
         * @return false coordinates are different
         */
        bool operator==(const Cell& other) const;

        // // for debugging
        // void printCoordinates() const {
        //     std::cout << "(" << x << "," << y << ")(" << number << ") \n";
        // }

        u_int16_t x, y;  // coordinates
        
    private:
        bool isFixed;           // is the cell fixed (given as input)?
};       


/**
 * @brief Class which contains all data about the puzzle
 * 
 */
class ThePuzzle {
    private:
        u_int16_t width;   // and width
        u_int16_t height;  // actual height

        // SAT
        u_int16_t totalLines;
        
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

        /**
         * @brief 
         * 
         * @return true 
         * @return false 
         */
        bool isSolved(); // check if the puzzle is solved
        
        /**
         * @brief 
         * 
         * @param toNumber 
         * @param start 
         * @param end 
         */
        void switchFinder(u_int16_t toNumber, Cell& start, Cell& end);
        
        /**
         * @brief Get the the number of edges in the puzzle
         * 
         * @return The number of edges
         */
        size_t getNumEdges() const {
            return ((width - 1) * height) + ((height - 1) * width);
        }

        /**
         * @brief Get the Literals object
         * 
         * @return size_t 
         */
        size_t getLiterals() const { return totalLines;}
        literals lit; // Struct in which contains all the literals

        /**
         * @brief Attaches a line literal in the direction the line is pointed to
         * 
         * @param addedCell The to be added cell
         * @param dir The direction the cell points from
         */
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
            std::pair<u_int16_t,u_int16_t>>> numberPairs; // vector of pairs of coordinates of numbers
        
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
        /**
         * @brief Will recursively solve the puzzle using DFS from the current cell;
         * 
         * @param curr The current cell
         * @param otherPair The other pair cell
         * @param p The Puzzle
         * @param currPair The current cell
         * @return true 
         * @return false 
         */
        bool solve(Cell* curr, Cell* otherPair, ThePuzzle &p, u_int16_t currPair) override;
        
        /**
         * @brief Wrapper function of the solver
         * 
         * @param p The puzzle
         */
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
         * @brief Encodes the puzzle into CNF
         * 
         * @param p The puzzle
         * @param width The width of the puzzle
         * @param height The height of the puzzle
         */
        void generateCNF(ThePuzzle& p, u_int8_t width, u_int8_t height);
};




#endif // PUZZLE_H