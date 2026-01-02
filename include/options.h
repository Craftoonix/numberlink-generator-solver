#ifndef options_h
#define options_h

#include <string>

static bool SHOW_INITIAL_PUZZLE = false;
static bool SOLVE_PUZZLE = false;
static bool USE_INPUT_FILE = false;
static bool USE_OUTPUT_FILE = false;
static bool GENERATE_PUZZLE = false;
static bool USE_HEURISTICS = false;
static bool EXPERIMENTAL_MODE = false;

static int SEED = 0;
static u_int16_t N = 1;
static u_int16_t ENABLED_HEURISTICS = 0;

static std::string INPUT_FILE = "";
static std::string OUTPUT_FILE = "";

#endif /* options_h */