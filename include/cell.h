#ifndef CELL_H
#define CELL_H

#include <string>
#include "puzzle.h"

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
    Line* lines[MAX_DIRECTIONS];         // which lines are connected to the cell
    
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
    
#endif