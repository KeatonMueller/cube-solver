#ifndef SOLVER_OLL_H
#define SOLVER_OLL_H

#include "Solver.h"

typedef Cube::LOCATION LOCATION;
typedef Cube::COLOR COLOR;
typedef Cube::FACE FACE;

/**
* Orient the last layer on the given cube.
*/
void solveOLL(Cube* cube);

/**
* The number of unique configurations for the
* up face to be in when it is time to solve OLL.
*
* This does not distinguish between the adjacent
* stickers, which is why it is not the number
* of total OLLs (57).
*/
const int NUM_OLL_TYPES = 29;

/**
* Values corresponding to all possible configurations of the up
* face when it is time to solve OLL.
*
* A 1 bit indicates the cross color is facing up, and a 0 bit indicates it is not.
* If olls[i] & (1 << n) is not equal to zero, that means there's a cross color
* facing up at index n, using the same indexing that Cube.h does.
*
* They are listed in decreasing order of number of stickers facing up.
*
* The corresponding name for the OLL is commented on the same line,
* based on the names from this resource:
* https://www.cubeskills.com/uploads/pdf/tutorials/oll-algorithms.pdf
*/
const uint8_t olls[NUM_OLL_TYPES] = {
	(1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 7),	// OCLL3/4
	(1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 7),	// OCLL5
	(1 << 0) | (1 << 1) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 7),	// E1
	(1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 6) | (1 << 7),	// E2
	(1 << 0) | (1 << 1) | (1 << 3) | (1 << 5) | (1 << 7),	// OCLL6/7
	(1 << 1) | (1 << 3) | (1 << 5) | (1 << 7),	// OCLL1/2
	(1 << 2) | (1 << 3) | (1 << 4) | (1 << 7),	// T1/2
	(1 << 0) | (1 << 2) | (1 << 3) | (1 << 7),	// C1/2
	(1 << 0) | (1 << 4) | (1 << 5) | (1 << 7),	// W1/2
	(1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),	// P1/3
	(1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),	// P2/4	
	(1 << 0) | (1 << 1) | (1 << 4) | (1 << 7),	// F3/4
	(1 << 0) | (1 << 2) | (1 << 5) | (1 << 7),	// A1/4
	(1 << 1) | (1 << 4) | (1 << 6) | (1 << 7),	// A2/3
	(1 << 2) | (1 << 3) | (1 << 6) | (1 << 7),	// B5
	(1 << 0) | (1 << 3) | (1 << 4) | (1 << 7),	// B6
	(1 << 0) | (1 << 2) | (1 << 4) | (1 << 6),	// O8
	(1 << 1) | (1 << 2) | (1 << 3),	// S1/2
	(1 << 1) | (1 << 4) | (1 << 7),	// F1/2
	(1 << 3) | (1 << 6) | (1 << 7),	// K1/4
	(1 << 3) | (1 << 4) | (1 << 7),	// K2/3
	(1 << 1) | (1 << 6) | (1 << 7),	// B1/4
	(1 << 0) | (1 << 5) | (1 << 7),	// B2/3
	(1 << 3) | (1 << 7),	// I1/2/3/4
	(1 << 1) | (1 << 7),	// L1/2/3/4/5/6
	(1 << 0) | (1 << 2),	// O6/7
	(1 << 0) | (1 << 4),	// O5
	(1 << 0),	// O3/4
	0	// O1/2
};

#endif
