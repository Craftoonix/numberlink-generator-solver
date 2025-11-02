#ifndef PUZZLE_H
#define PUZZLE_H

#include "constants.h"
#include <vector>
#include <utility>

/**
 * @brief Cardinal Directions of a cell
 * 
 */
enum Direction {UP, RIGHT, DOWN, LEFT, NOP};


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
        Direction path;                      // direction towards next cell which will form a path

        // Kruskal's algorithm members
        Cell* inPath;              // pointer to the cell where the path comes from
        Cell* outPath;             // pointer to the cell where the path goes to
        
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
        
        /**
         * @brief Create the grid of the puzzle using linked lists
         * 
         */
        void createGrid();
        
    public:
        u_int16_t numPairs; // number of pairs of numbers
        ThePuzzle(u_int16_t w, u_int16_t h, 
            std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
            std::pair<u_int16_t,u_int16_t>>> n);
        ~ThePuzzle ();
            
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
#endif // PUZZLE_H;