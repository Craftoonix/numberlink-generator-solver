#include "cell.h"

Cell::Cell(u_int16_t x_coord, u_int16_t y_coord)                                          
{
    for (size_t i = 0; i < MAX_DIRECTIONS; i++) {
        adjacent[i] = nullptr;
        lines[i] = nullptr;
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