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
* Solve the given Rubik's Cube.
*/
void solve(Cube* cube)
{
	COLOR color = COLOR::WHITE;
	solveCross(cube, color);
	solveCorners(cube, color);
	solveSecondLayer(cube);
	// solve OLL
	// solve PLL
}
