#include <string>
#include <iostream>

#include "Solver.h"

int main()
{
	Cube c;
	c.print();
	c.readMoves("FFBBDDFL'BR'UD'R'D'R'FL'F'");
	std::cout << "Scrambled cube:" << std::endl;
	c.print();

	solve(&c);
	std::cout << "Solved cube:" << std::endl;
	c.print();
}
