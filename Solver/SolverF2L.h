#ifndef SOLVER_F2L_H
#define SOLVER_F2L_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Solve the first two layers on the given cube.
*/
void solveF2L(Cube& cube, std::vector<Move>& solution);

#endif
