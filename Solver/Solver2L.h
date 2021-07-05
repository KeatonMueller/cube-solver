#ifndef SOLVER_2L_H
#define SOLVER_2L_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Solve the second layer on the given cube.
*/
void solveSecondLayer(Cube& cube, std::vector<Move>& solution);

#endif