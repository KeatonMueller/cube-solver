#include <iostream>

#include "Solver.h"

/**
 * Find which layer the given location is in.
 */
LAYER getLayer(LOCATION l)
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
 * Clean the given solution by continually merging
 * adjacent moves until no more merges are possible.
 *
 * Optionally perform an optimized clean where the
 * newline markers are moved from the solution, potentially
 * leading to shorter solution lengths.
 */
std::vector<Move> cleanSolution(std::vector<Move>& solution, bool optimized)
{
	std::vector<Move> cleaned;
	while (solution.size() > 0)
	{
		// if cleaned is empty, add first element from solution
		if (cleaned.size() == 0)
			cleaned.push_back(solution[0]);
		// otherwise
		else
		{
			// check if end of cleaned can merge with front of solution
			if (cleaned[cleaned.size() - 1].canMergeWith(solution[0]))
			{
				// get merged move
				Move mergedMove = cleaned[cleaned.size() - 1].merge(solution[0]);
				// delete old end of cleaned
				cleaned.erase(cleaned.end() - 1, cleaned.end());
				// only add to cleaned if merged move is not NO_MOVE
				if (mergedMove.type != Move::TYPE::NO_MOVE)
					cleaned.push_back(mergedMove);
			}
			// if not, add all moves if unoptimized, or add all non NO_MOVE moves if optimized
			else if (!optimized || solution[0].type != Move::TYPE::NO_MOVE)
				cleaned.push_back(solution[0]);
		}
		// delete old start of solution
		solution.erase(solution.begin(), solution.begin() + 1);
	}
	return cleaned;
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
 * Return a string value of the solution.
 */
std::string solutionToString(std::vector<Move>& solution)
{
	std::string str;

	for (Move& move : solution)
	{
		// don't add moves of type no move
		if (move.type != Move::TYPE::NO_MOVE)
			str += move.toString() + " ";
	}

	return str;
}

/**
 * Solve the given Rubik's Cube.
 */
std::vector<Move> solve(Cube& cube)
{
	// vector of moves in the solution
	std::vector<Move> solution;

	solveCross(cube, solution);
	solveF2L(cube, solution);
	solveOLL(cube, solution);
	solvePLL(cube, solution);

	return cleanSolution(solution);
}
