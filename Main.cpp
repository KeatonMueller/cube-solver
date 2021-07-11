#include <iostream>
#include <numeric>
#include <string>
#include <time.h>

#include "Solver.h"

/**
 * Generate a random scramble.
 */
std::string generateScramble()
{
	// moves to choose from
	std::string moves[18] = { "U", "U'", "U2", "D", "D'", "D2", "F", "F'", "F2", "B", "B'", "B2", "R", "R'", "R2", "L", "L'", "L2" };
	std::string scramble = "";
	// randomly select 25 moves
	for (uint8_t i = 0; i < 25; i++)
	{
		int idx = rand() % 18;
		scramble += moves[idx] + " ";
	}
	return scramble;
}

/**
 * Test the solver on a sequence of random scrambles.
 *
 * Stop and report a failed scramble if it's ever unable
 * to solve cube.
 *
 * Report average solution length at the end.
 *
 * In testing, it has never failed, and has been run on
 * over one million random scrambles.
 */
void testRandomScrambles()
{
	// set random seed
	srand((unsigned int)time(NULL));

	// store solution lengths
	std::vector<size_t> solutionLengths;

	// perform random tests
	Cube c;
	std::string scramble;
	for (int i = 0; i < 50000; i++)
	{
		// reset the cube and scramble it
		c.reset();
		scramble = generateScramble();
		c.readMoves(scramble);
		// solve it
		std::cout << i << " attempting " << scramble << "...";
		std::vector<Move> solution = solve(c);
		if (!c.isSolved())
		{
			std::cout << "Failed to solve: " << scramble << std::endl;
			break;
		}
		else
		{
			std::cout << "Solved!" << std::endl;
			// verify solve (for sanity)
			c.reset();
			c.readMoves(scramble);
			solution = cleanSolution(solution, true);
			c.executeMoves(solution);
			if (!c.isSolved())
			{
				std::cout << "Failed to replicate solve: " << scramble << std::endl;
				break;
			}
			// printSolution(solution);
			solutionLengths.push_back(solution.size());
		}
	}
	float avgLength = (float)std::accumulate(solutionLengths.begin(), solutionLengths.end(), 0) / solutionLengths.size();
	std::cout << "Average Solution Length: " << avgLength << std::endl;
}

int main(int argc, char* argv[])
{
	// get a scramble from the user
	std::string scramble;
	std::cout << "Enter scramble: ";
	std::getline(std::cin, scramble);

	// get the solution
	Cube c;
	c.readMoves(scramble);

	std::vector<Move> solution = solve(c);
	std::cout << "\nSolution:\n\n";
	printSolution(solution);

	solution = cleanSolution(solution, true);
	std::cout << "\nOptimized:\n\n";
	printSolution(solution);

	std::cout << "\nPress Enter to exit" << std::endl;
	std::cin.get();

	return 0;
}
