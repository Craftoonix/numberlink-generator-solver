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

-i: Shows the (i)nitial puzzle
-s \<METHOD\>: (S)olves the puzzle using DFS or SAT
-f \<FILE\>: Make use of a configuration (f)ile instead of configuring in the commandline
-g \<METHOD\>: (G)enerate a puzzle using method SAT
-r \<SEED\>: Set seed for the (R)andom Number Generator
-h: Make use of (H)euristics for the possibility of generating puzzles faster

### Configuration
