# include <iostream>
# include <string>
# include <fstream>
# include <cstdlib>
# include <ctime>
# include "puzzle.h"
# include "constants.h"

// main program
int main (int argc, char* argv[ ]) {
  if ( argc != ARGUMENTS + 1 ) {
    std::cout << "Usage: " << argv[0] 
	 << " " << std::endl;
    return EXIT_FAILURE;
  }//if
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  if ( width <= 0 || height <= 0 ) {
    std::cerr << "Error: width & height must be positive integers" << std::endl;
    return EXIT_FAILURE;
  }
  if ( width > MAX_CELLS || height > MAX_CELLS ) {
    std::cerr << "Error: maximum width & height is " << MAX_CELLS << std::endl;
    return EXIT_FAILURE; 
  }


  ThePuzzle numberlink((u_int16_t)width, u_int16_t(height));
  numberlink.printPuzzle();
  
  return EXIT_SUCCESS;
}//main
