#include <string>
#include <iostream>

#include "Solver.h"

int main()
{
	Cube c;
	c.print();
	c.readMoves("ZZRU'R'U'F'UFUUURU'R'U'F'UF");
	std::cout << "Scrambled cube:" << std::endl;
	c.print();

	solve(&c);
	std::cout << "Solved cube:" << std::endl;
	c.print();
}
