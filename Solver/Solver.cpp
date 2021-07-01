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
* Clean the given solution by merging adjacent moves
* if possible.
*/
void cleanSolution(std::vector<Move>& solution)
{
	for (int i = 0; i < solution.size() - 1; i++)
	{
		if (solution[i].canMergeWith(solution[i + 1]))
		{
			Move newMove = solution[i].merge(solution[i + 1]);
			solution.erase(solution.begin() + i, solution.begin() + i + 2);
			solution.insert(solution.begin() + i, newMove);
			i--;
		}
	}
}

/**
* Print the given solution.
*/
void printSolution(std::vector<Move>& solution)
{
	for (auto& move : solution)
	{
		// don't print moves of type no move
		if (move.type != Move::TYPE::NO_MOVE)
			std::cout << move.toString() << " ";
		// encode pieces = Y and type = no move to mean add a newline
		else if (move.pieces == Move::PIECES::Y)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

/**
* Solve the given Rubik's Cube.
*/
std::vector<Move> solve(Cube* cube)
{
	// vector of moves in the solution
	std::vector<Move> solution;

	COLOR color = COLOR::WHITE;
	solveCross(cube, color, solution);
	solveCorners(cube, color, solution);
	solveSecondLayer(cube, solution);
	solveOLL(cube, solution);
	solvePLL(cube, solution);

	cleanSolution(solution);

	return solution;
}
