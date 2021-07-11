# cube-solver

This is a C++ application to solve a Rubik's Cube.

The method used by this program is the CFOP method, which you
can learn about [here](https://ruwix.com/the-rubiks-cube/advanced-cfop-fridrich/).

The average solution length is around 70 moves, using half turn metric. While
this is far from the optimal number of 20, it isn't terribly far off from what
human speedcubers can achieve using CFOP, which is usually around 50-60.

## How it works

When you run the program, you will be prompted to enter a scramble.

Scrambles should follow standard cube notation, which you can read
about [here](https://ruwix.com/the-rubiks-cube/notation/).

The cube can be in any orientation at the time of the scramble, and whichever
color ends up at the bottom once the cube is finished scrambling is chosen
to be the cross color.

The program will output two solutions, one regular and one "optimized".

The regular solution has line breaks after each step (cross piece, F2L pair, etc) to make it
easier to read. However, this can potentially lead to inefficiencies in the solution
if for example one step ends with `U` and the next one begins with `U'`.

The optimized solution has no line breaks, and therefore is able to merge adjacent moves
that can be condensed or eliminated entirely, like in the earlier example. It is not a
fundamentally different solution, however.

## How to use it

### Windows

On Windows, you can run the `cube-solver.exe` executable found in the root
of this repository.

### MacOS

On Mac, you can run the `cube-solver` executable found in the root of this
repository.

### Web

This project has also been compiled to web assembly and powers the solution mechanism
on my web-based 3D virtual Rubik's Cube.

You can see that project running [online](https://www.keatonmueller.com/cube.html).

To learn the controls and how it works, you can see the project on [GitHub](https://github.com/KeatonMueller/cube).

### Compilation

To compile this program yourself, you'll need `make` and the `g++` compiler.

If you have them, simply run `make` from the project's root directory.
