# include <iostream>
# include <string>
# include <fstream>
# include <cstdlib>
# include <ctime>
# include "puzzle.h"
# include "constants.h"

// main program
int main (int argc, char* argv[ ]) {
    // check command line arguments
    if ((argc - 3) % 4 != 0) {
        std::cout << "Usage: " << argv[0] 
        << "<width> <height> <coordinates of pairs>" << std::endl;
        return EXIT_FAILURE;
    }//if
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    if (width <= 0 || height <= 0) {
        std::cerr << "Error: width & height must be positive integers" << std::endl;
        return EXIT_FAILURE;
    }
    if (width > MAX_CELLS || height > MAX_CELLS) {
        std::cerr << "Error: maximum width & height is " << MAX_CELLS << std::endl;
        return EXIT_FAILURE; 
    }

    //create vector of pairs of coordinates of numbers
    std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
                           std::pair<u_int16_t,u_int16_t>>> numberPairs;
    for (int i = 0; i < argc - 3; i = i + 4)
    {
        int x1 = atoi(argv[i + 3]);
        int y1 = atoi(argv[i + 4]);
        int x2 = atoi(argv[i + 5]);
        int y2 = atoi(argv[i + 6]);
        if (x1 < 0 || x1 >= width || y1 < 0 || y1 >= height ||
            x2 < 0 || x2 >= width || y2 < 0 || y2 >= height) {
        std::cerr << "Error: coordinates must be within the grid" << std::endl;
        return EXIT_FAILURE; 
        }
        // pairs must be unique
        for (const auto& p : numberPairs) {
            if ((p.first.first == x1 && p.first.second == y1) ||
                (p.second.first == x1 && p.second.second == y1) ||
                (p.first.first == x2 && p.first.second == y2) ||
                (p.second.first == x2 && p.second.second == y2)) {
                std::cerr << "Error: pairs must be unique" << std::endl;
                return EXIT_FAILURE;
            }
        }
        // and cant be the same
        if (x1 == x2 && y1 == y2) {
            std::cerr << "Error: a pair cannot have the same coordinates" << std::
            endl;
            return EXIT_FAILURE;
        }

        // add the pair to the vector
        numberPairs.push_back(std::make_pair(std::make_pair(x1,y1),
                                            std::make_pair(x2,y2)));
    }
    
    ThePuzzle numberlink((u_int16_t)width, u_int16_t(height), numberPairs);
    numberlink.printPuzzle();
    
    return EXIT_SUCCESS;
}//main
