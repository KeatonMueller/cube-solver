#include <string>
#include <iostream>

#include "Solver.h"

int main()
{
	Cube c;
	c.print();
	c.readMoves("BLBR'U'R'F'DL'D'BUURRBBFFU'BBUBBFFD'");
	std::cout << "Scrambled cube:" << std::endl;
	c.print();

	solve(&c);
	std::cout << "Solved cube:" << std::endl;
	c.print();
}
