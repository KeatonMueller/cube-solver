#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include "Cube.h"
#include "Move.h"
#include "SolverCross.h"
#include "SolverCorners.h"
#include "Solver2L.h"
#include "SolverOLL.h"
#include "SolverPLL.h"

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
* Print the given solution.
*/
void printSolution(std::vector<Move>& solution);

/**
* Solve the given Rubik's Cube!
*/
std::vector<Move> solve(Cube* cube);

#endif
