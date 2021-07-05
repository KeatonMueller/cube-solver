#ifndef SOLVER_CORNERS_H
#define SOLVER_CORNERS_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Solve the corners on the given cube.
*/
void solveCorners(Cube& cube, std::vector<Move>& solution);

#endif