#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>

#include "Cube.h"
#include "Move.h"
#include "SolverCross.h"
#include "SolverF2L.h"
#include "SolverOLL.h"
#include "SolverPLL.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
 * 8-bit enum for each horizontal layer
 */
enum class LAYER : uint8_t { BOTTOM, MIDDLE, TOP };

/**
 * Find which layer the given location is in.
 */
LAYER getLayer(LOCATION l);

/**
 * Clean the given solution, removing redundancies.
 */
std::vector<Move> cleanSolution(std::vector<Move>& solution, bool optimized = false);

/**
 * Print the given solution.
 */
void printSolution(std::vector<Move>& solution);

/**
 * Return a string value of the solution.
 */
std::string solutionToString(std::vector<Move>& solution);

/**
 * Solve the given Rubik's Cube!
 */
std::vector<Move> solve(Cube& cube);

#endif
