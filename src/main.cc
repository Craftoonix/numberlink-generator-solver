#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "constants.h"
#include "options.h"
#include "sat.h"


enum solverPrograms
{
    DFS = 1,
    SAT = 2,
};


// main program
int main (int argc, char* argv[]) {
    int opt;
    u_int8_t solverProgram = 0;
    // parse command line options
    while ((opt = getopt(argc, argv, ":s:i")) != -1) 
    {
        switch (opt)
        {
        case 's':
            // solve the puzzle
            SOLVE_PUZZLE = true;
            //std::string arg(optarg);
            if (strcmp(optarg, "dfs") == 0) 
                solverProgram = DFS;
            else if (strcmp(optarg, "sat") == 0)
                solverProgram = SAT;
            else {
                std::cerr << "Error: unknown solving method '" << optarg << "'\n";
                return EXIT_FAILURE;
            }
            break;
        case 'i':
            // show the initial puzzle
            SHOW_INITIAL_PUZZLE = true;
            break;
        
        case '?':
            std::cerr << "Unknown option: " << char(optopt) << std::endl;
        }
    }

    // after options, there should be width, height, and pairs
    if ((argc - optind + 2) % 4 != 0) {
        std::cout << "Usage: " << argv[0] 
        << " <options> <width> <height> <coordinates of pairs>" << std::endl;
        return EXIT_FAILURE;
    }

    int width = atoi(argv[optind]);
    int height = atoi(argv[optind + 1]);
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
    for (int i = optind + 2; i < argc; i = i + 4)
    {
        int x1 = atoi(argv[i + 0]);
        int y1 = atoi(argv[i + 1]);
        int x2 = atoi(argv[i + 2]);
        int y2 = atoi(argv[i + 3]);
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
    if (SHOW_INITIAL_PUZZLE) {
        std::cout << "Initial puzzle:\n";
        numberlink.printPuzzle();
        std::cout << std::endl;
    }
    if (!SOLVE_PUZZLE) {
        return EXIT_SUCCESS; // nothing more to do
    }

    if (solverProgram == DFS) {
        dfs DFSsolver;
        DFSsolver.solveWrapper(numberlink);
    }
    else if (solverProgram == SAT)
    {
        sat SATsolver;
        SATsolver.solve(numberlink,width,height, numberPairs.size());
    }

    if (!numberlink.isSolved()) {
        std::cout << "No solution found.\n";
        return EXIT_FAILURE;
    }
    std::cout << "Solved puzzle:\n";
    numberlink.printPuzzle();
    
    return EXIT_SUCCESS;
}//main
