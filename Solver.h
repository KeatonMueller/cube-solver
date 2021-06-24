#ifndef SOLVER_H
#define SOLVER_H

#include "Cube.h"
#include "SolverCross.h"
#include "SolverCorners.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* 8-bit enum for each horizontal layer
*/
enum class LAYER : uint8_t
{
	BOTTOM,
	MIDDLE,
	TOP
};

/**
* Find which layer the given location is in.
*/
LAYER getLayer(Cube* c, LOCATION l);

/**
* Solve the given Rubik's Cube!
*/
void solve(Cube* cube);

#endif
