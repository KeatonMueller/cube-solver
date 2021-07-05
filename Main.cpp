#include <string>
#include <iostream>

#include "Solver.h"

/**
* Generate a random scramble.
*/
std::string generateScramble()
{
	// moves to choose from
	std::string moves = "UDFBRLMESXYZ";
	std::string scramble = "";
	// randomly select 30 moves
	for (uint8_t i = 0; i < 30; i++)
	{
		int idx = rand() % moves.length();
		scramble += moves.substr(idx, 1);
	}
	return scramble;
}

/**
* Test the solver on a sequence of random scrambles.
*
* Stop and report a failed scramble if it's ever unable
* to solve cube.
*
* In testing, it has never failed, and has been run on
* over one million random scrambles.
*/
void testRandomScrambles()
{
	// set random seed
	srand((unsigned int)time(NULL));

	// perform random tests
	Cube c;
	std::string scramble;
	for (int i = 0; i < 10000; i++)
	{
		// reset the cube and scramble it
		c.reset();
		scramble = generateScramble();
		c.readMoves(scramble);
		// solve it
		std::vector<Move> solution = solve(c);
		if (!c.isSolved())
		{
			std::cout << "Failed to solve: " << scramble << std::endl;
			break;
		}
		else
		{
			std::cout << "Solved scramble " << i << std::endl;
			// printSolution(solution);
		}
	}
}

int main()
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
}
