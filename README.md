# numberlink-generator-solver

## Dependencies

Minisat is required for this program to run.
Install it using:

### Arch

```bash
sudo pacman -S minisat
```

### Debian/Ubuntu

```bash
sudo apt install minisat
```

### Fedora

```bash
sudo dnf minisat
```

## Building

Build the program using:

```bash
make
```

## Removing

Remove the program using:

```bash
make clean
```

## Usage

```bash
./numberlink <OPTIONS> <WIDTH> <HEIGHT> <CONFIGURATION>
```

### Options

- -i: Shows the (I)nitial puzzle
- -s \<METHOD\>: (S)olves the puzzle using DFS or SAT
- -f \<FILE\>: Make use of a configuration (f)ile instead of configuring in the commandline
- -g \<METHOD\>: (G)enerate a puzzle using method SAT
- -r \<SEED\>: Set seed for the (R)andom Number Generator
- -h \<HEURISTIC\>: Make use of (H)euristics for the possibility of generating puzzles faster
- -e: Show (E)xperimental data, such as the time it took for the operation to complete on average
- -n \<REPETITIONS\>: Repeat (N) times
- -o \<FILE\>: Set output file when generating puzzles

#### Methods

Right now the only methods available for solving a puzzle are sat and dfs. 
The only method of generating a puzzle is sat

#### Heuristic

- 1: Enable only the encased cell heuristic
- 2: Enable only the parity heuristic
- 3: Enable both the encased cell and parity heuristic

### Configuration

The configuration represent the pairs and their positions. For each added pair, 4 numbers need to be added.
The first number will be the X coordinate of the first cell and the second number the Y coordinate.
The third will be the X coordinate of the other half and the second number the Y coordinate.
The coordinate system is as follows: the upper left cell is coordinate (0,0). X increases for each row to the right and Y increases for each collumn downwards.

## Examples

```bash
./numberlink -i -s dfs 4 4 0 0 3 3 3 0 3 1 1 2 3 2 
```
The options dictate that the terminal will output the initial puzzle and our program will solve it using DFS. The configuration tells the program that the puzzle has a size of 4x4, the 2 coordinates of the first
pair are (0,0) and (3,3), the second pair: (3,0) and (3,1), the third: (1,2) and (3,2).

```bash
./numberlink -s sat -n 100 -f example_puzzles/size10/pairs8.txt
```
This will solve 100 puzzles from this file using SAT.

```bash
./numberlink -g sat -h 3 -i -o output.txt 5 5 4
```
Generate a random 5x5 puzzle containing 4 pairs using sat with both heuristics enabled, show the generated puzzle, and output it to output.txt.

```bash
./numberlink -g sat -r 42 -h 2 -n 100 -e 10 10 6
```
Generate a random 10x10 puzzle containing 6 pairs using sat with only the parity heuristic enabled on seed 42 100 times and show the experimental data.