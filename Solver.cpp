#include "Solver.h"

/**
* Return the location of an unsolved edge piece of the given color.
* Second item in pair is false if no more unsolved edge pieces remain.
*/
std::pair<LOCATION, bool> findUnsolvedEdge(Cube cube, COLOR color)
{
	// search every sticker on every face
	for (uint8_t face = 0; face < 6; face++)
	{
		for (uint8_t idx = 0; idx < 8; idx++)
		{
			if (cube.getSticker({ (FACE)face, idx }) == color)
				return std::make_pair(LOCATION({ (FACE)face, idx }), true);
		}
	}
	return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
}

void solveCrossPiece(Cube cube, LOCATION piece)
{

}


/**
* Solve the cross of the given color on the given cube.
*
* Assumes that no part of the cube is solved.
* The cube need not be oriented so that the cross color is on
* bottom.
*/
void solveCross(Cube cube, COLOR color)
{
	// orient the cube so the cross color is on the bottom

	// solve cross

}

/**
* Solve the given Rubik's Cube using the CFOP method.
*/
void solve(Cube cube)
{
	solveCross(cube, COLOR::WHITE);
}
