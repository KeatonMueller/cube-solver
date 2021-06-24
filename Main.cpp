#include <string>
#include <iostream>

#include "Solver.h"

int main()
{
	Cube c;
	c.print(); // BLBR'U'R'F'DL'D'BUURRBBFFU'BBUBBFFD'
	c.readMoves("ZZLUL'U'LUL'U'LUL'U'");
	std::cout << "Scrambled cube:" << std::endl;
	c.print();

	solve(&c);
	std::cout << "Solved cube:" << std::endl;
	c.print();
}
