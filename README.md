# cube-solver

This is a C++ application to solve a Rubik's Cube.

The method used by this program is the CFOP method, which you
can learn about [here](https://ruwix.com/the-rubiks-cube/advanced-cfop-fridrich/).

The average solution length is around 70 moves, using half turn metric. While
this is far from the optimal number of 20, it isn't terribly far off from what
human speedcubers can achieve using CFOP, which is usually around 50-60.

## How to use it

To compile this program, simply run `make` from the project's root directory.

Alternatively, on Mac you can run the `cube-solver` executable, and on Windows
you can run the `cube-solver.exe` executable.

When you run the program, you will be prompted to enter a scramble.

Scrambles should follow standard cube notation, which you can read
about [here](https://ruwix.com/the-rubiks-cube/notation/).

The cube can be in any orientation at the time of the scramble, and whichever
color ends up at the bottom once the cube is finished scrambling is chosen
to be the cross color.
