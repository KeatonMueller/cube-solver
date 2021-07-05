#ifndef SOLVER_OLL_H
#define SOLVER_OLL_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Orient the last layer on the given cube.
*/
void solveOLL(Cube& cube, std::vector<Move>& solution);

#endif
