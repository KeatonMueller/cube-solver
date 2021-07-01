#ifndef SOLVER_CORNERS_H
#define SOLVER_CORNERS_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Solve the corners for the given color
*/
void solveCorners(Cube* cube, COLOR color, std::vector<Move>& solution);

#endif