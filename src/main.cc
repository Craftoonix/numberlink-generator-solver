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
  u_int8_t widthh = atoi(argv[1]);
  u_int8_t heighth = atoi(argv[2]);

  ThePuzzle numberlink(widthh, heighth);
  
  return EXIT_SUCCESS;
}//main
