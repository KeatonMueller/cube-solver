#ifndef SOLVER_PLL_H
#define SOLVER_PLL_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Permute the last layer on the given cube.
*/
void solvePLL(Cube& cube, std::vector<Move>& solution);

#endif
