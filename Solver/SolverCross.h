#ifndef SOLVER_CROSS_H
#define SOLVER_CROSS_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Solve the cross of the given color on the given cube.
*/
void solveCross(Cube* cube, COLOR color);

#endif
