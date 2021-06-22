#include <iostream>

#include "Solver.h"

/**
* Find which layer the given location is in.
*/
LAYER getLayer(Cube* c, LOCATION l)
{
	if (l.face == FACE::DOWN)
		return LAYER::BOTTOM;
	else if (l.face == FACE::UP)
		return LAYER::TOP;
	else if (l.idx < 3)
		return LAYER::TOP;
	else if (3 < l.idx && l.idx < 7)
		return LAYER::BOTTOM;
	else
		return LAYER::MIDDLE;
}

/**
* Solve the given Rubik's Cube using the CFOP method.
*/
void solve(Cube* cube)
{
	solveCross(cube, COLOR::WHITE);
	// solve F2L
	// solve OLL
	// solve PLL
}
