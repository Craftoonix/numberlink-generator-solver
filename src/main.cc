#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "constants.h"
#include "options.h"
#include "sat.h"

enum class genPrograms
{
    SAT,
} genProgram;

enum class solverPrograms
{
    DFS,
    SAT,
} solverProgram;

void showUsage(std::string programName)
{
    std::cout << "Usage: " << programName
    << " <options> <width> <height> <coordinates of pairs>" << std::endl;
    std::cout << "example: " << programName << "-i -s sat 4 4 0 0 3 3 3 0 3 1 1 2 2 3 3 2" << std::endl;
    std::cout << "This will show the initial puzzle first and then solve it using SAT" << std::endl;
    std::cout << "This puzzle is a 4x4 puzzle with the following number pairs:" << std::endl;
    std::cout << "1: on coordinates (0.0) and (3.3)" << std::endl;
    std::cout << "2: on coordinates (3.0) and (3.1)" << std::endl;
    std::cout << "3: on coordinates (1.2) and (3.2)" << std::endl;
    std::cout << "The coordinates are formed as (x,y)" << std::endl;
    std::cout << "The x coordinate represents the row the number is in and increases every row to the right" << std::endl;
    std::cout << "The y coordinate represents the collumn the number is in and increases every collumn downwards" << std::endl;
}

bool parseInputFile(int & nArgs, u_int16_t args[])
{
    std::ifstream file(INPUT_FILE);
    std::string line;
    std::stringstream coords;
    u_int16_t coord = 0;
    u_int16_t counter = 0;


    if(!file.is_open()) {
        std::cerr << "ERROR: Input file not found\n";
        return EXIT_FAILURE;
    }

    // gather all lines from input file
    while (getline(file,line))
        coords << line;   
    file.close();

    // parse line to form coordinates
    while (coords >> coord)
    {
        args[counter] = coord;
        counter++;
    }
    nArgs = counter;
    return 0;
}


// main program
int main (int argc, char* argv[]) {
    int opt;
    int nArgs = argc;
    int width, height;
    int x1, x2, y1, y2;
    u_int16_t args[MAX_CELLS * MAX_CELLS];
    //create vector of pairs of coordinates of numbers
    std::vector<std::pair<std::pair<u_int16_t,u_int16_t>,
                            std::pair<u_int16_t,u_int16_t>>> numberPairs;


    // parse command line options
    while ((opt = getopt(argc, argv, ":s:g:r:if:")) != -1) 
    {
        switch (opt)
        {
        case 's':
            // solve the puzzle
            SOLVE_PUZZLE = true;
            if (strcmp(optarg, "dfs") == 0) 
                solverProgram = solverPrograms::DFS;
            else if (strcmp(optarg, "sat") == 0)
                solverProgram = solverPrograms::SAT;
            else {
                std::cerr << "Error: unknown solving method '" << optarg << "'\n";
                return EXIT_FAILURE;
            }
            break;
        case 'i':
            // show the initial puzzle
            SHOW_INITIAL_PUZZLE = true;
            break;
        case 'f':
            INPUT_FILE = optarg;
            USE_INPUT_FILE = true;
            //optind = 0;
            if (parseInputFile(nArgs, args)) return EXIT_FAILURE;
            break;
        case 'r':
            SEED = atoi(optarg);
            break;
        case 'g':
            // generate a puzzle
            GENERATE_PUZZLE = true;
            if (strcmp(optarg, "sat") == 0)
                genProgram = genPrograms::SAT;
            else {
                std::cerr << "Error: unknown generator method '" << optarg << "'\n";
                return EXIT_FAILURE;
            }
            break;
        case '?':
            std::cerr << "Unknown option: " << char(optopt) << std::endl;
        }
    }

    // after options, there should be width, height, and pairs
    if (!USE_INPUT_FILE) {
        if (GENERATE_PUZZLE && ((argc - optind - 3) != 0)){
            showUsage(argv[0]);
            return EXIT_FAILURE;
        }
        if (!GENERATE_PUZZLE && ((argc - optind + 2) % 4 != 0)) {
            showUsage(argv[0]);
            return EXIT_FAILURE;
        }
    }
    else {
        if (GENERATE_PUZZLE && (nArgs + 3 != 0)) {
            showUsage(argv[0]);
            return EXIT_FAILURE;
        }
        if (!GENERATE_PUZZLE && ((nArgs + 2) % 4 !=0)) {
            showUsage(argv[0]);
            return EXIT_FAILURE;
        } 
    }
    width = (USE_INPUT_FILE) ? args[0] : atoi(argv[optind]);
    height = (USE_INPUT_FILE) ? args[1] : atoi(argv[optind + 1]) ;
    if (width <= 0 || height <= 0) {
        std::cerr << "Error: width & height must be positive integers" << std::endl;
        return EXIT_FAILURE;
    }
    if (width > MAX_CELLS || height > MAX_CELLS) {
        std::cerr << "Error: maximum width & height is " << MAX_CELLS << std::endl;
        return EXIT_FAILURE; 
    }

    if (GENERATE_PUZZLE){
        switch (genProgram)
        {
        case genPrograms::SAT:
            u_int16_t nPairs = (USE_INPUT_FILE) ? args[2] : atoi(argv[optind+2]);
            sat gen;
            while (true)
            {
                // generate a puzzle
                numberPairs = gen.generate(width,height,nPairs, SEED);
                ThePuzzle numberlink((u_int16_t)width,(u_int16_t)height,numberPairs);

                // check if it is solvable and redo if not
                gen.solve(numberlink,width,height,nPairs);
                if (numberlink.isSolved()) break;
            }
            break;
        }
    }
    else {
        for (int i = (USE_INPUT_FILE) ? 2 : optind + 2; i < nArgs; i = i + 4)
        {
            x1 = (USE_INPUT_FILE) ? args[i + 0] : atoi(argv[i + 0]);      
            y1 = (USE_INPUT_FILE) ? args[i + 1] : atoi(argv[i + 1]); 
            x2 = (USE_INPUT_FILE) ? args[i + 2] : atoi(argv[i + 2]); 
            y2 = (USE_INPUT_FILE) ? args[i + 3] : atoi(argv[i + 3]); 
    
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
    } // if not generate

    
    ThePuzzle numberlink((u_int16_t)width, u_int16_t(height), numberPairs);
    if (SHOW_INITIAL_PUZZLE) {
        std::cout << "Initial puzzle:\n";
        numberlink.printPuzzle();
        std::cout << std::endl;
    }
    if (!SOLVE_PUZZLE) {
        return EXIT_SUCCESS; // nothing more to do
    }

    if (solverProgram == solverPrograms::DFS) {
        dfs DFSsolver;
        DFSsolver.solveWrapper(numberlink);
    }
    else if (solverProgram == solverPrograms::SAT)
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
