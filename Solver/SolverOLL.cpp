#include <iostream>

#include "SolverOLL.h"

/**
* Generate a bit mask of the top face of a cube
* selecting every location as indicated by the given OLL.
*
* In addition, generate a bit mask of the same positions
* filled with a specified color.
*/
std::pair<uint64_t, uint64_t> generateOLLMask(COLOR color, uint8_t oll)
{
	uint64_t mask = 0;
	uint64_t face = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		if ((oll & (1 << i)) != 0)
		{
			mask |= (uint64_t)0xff << ((7 - i) * 8);
			face |= (uint64_t)color << ((7 - i) * 8);
		}
	}
	return std::make_pair(mask, face);
}

/**
* Generate a bit mask of the top row of the cube
* by selecting the given locations.
*
* In addition, generate a bit mask of the same positions
* filled with a specific color.
*/
std::pair<uint64_t, uint64_t> generateRowMask(COLOR color, bool left, bool middle, bool right)
{
	uint64_t mask = 0;
	uint64_t row = 0;
	if (left)
	{
		mask |= (uint64_t)0xff << 56;
		row |= (uint64_t)color << 56;
	}
	if (middle)
	{
		mask |= (uint64_t)0xff << 48;
		row |= (uint64_t)color << 48;
	}
	if (right)
	{
		mask |= (uint64_t)0xff << 40;
		row |= (uint64_t)color << 40;
	}
	return std::make_pair(mask, row);
}

/**
* Shift the integer representing an OLL
* by two bits to the left, wrapping around.
*
* This is equivalent to rotating the OLL shape
* clockwise once.
*/
uint8_t shiftOLL(uint8_t oll)
{
	uint8_t toSave = oll & ((1 << 6) | (1 << 7));
	return (oll << 2) | (toSave >> 6);
}

/**
* Determine if the given OLL matches the cube state.
*
* Returns a (boolean, uint8_t) pair where the boolean indicates
* whether or not the given OLL matches the cube, and the integer
* indicates the number of clockwise rotations needed for the OLL
* to align with the cube state.
*/
std::pair<bool, uint8_t> ollCaseFits(Cube* cube, COLOR topColor, uint8_t oll)
{
	// check all four orientations of the OLL
	for (uint8_t shifts = 0; shifts < 4; shifts++)
	{
		// generate mask
		std::pair<uint64_t, uint64_t> mask = generateOLLMask(topColor, oll);
		// return how many shifts needed if OLL case matches
		if ((cube->getFace(FACE::UP) & mask.first) == mask.second)
			return std::make_pair(true, shifts);
		// shift the OLL
		oll = shiftOLL(oll);
	}
	// no orientation worked, return false in first element of pair
	return std::make_pair(false, 0);
}

/**
* Find the type of OLL case this is, based on the number and position
* of the oriented top layer pieces.
*
* Returns a pair of the OLL followed by the number of clockwise rotations
* needed for the OLL to match the cube state.
*/
std::pair<uint8_t, uint8_t> findOLLType(Cube* cube, COLOR topColor)
{
	for (uint8_t idx = 0; idx < NUM_OLL_TYPES; idx++)
	{
		std::pair<bool, uint8_t> ollShifts = ollCaseFits(cube, topColor, olls[idx]);
		if (ollShifts.first)
			return std::make_pair(olls[idx], ollShifts.second);
	}
	// not possible since every OLL case is covered by the above for loop
	return std::make_pair(NUM_OLL_TYPES, 0);
}

/**
* Solve OLL cases OCLL3 and OCLL4
*/
void solveOCLL34(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// determine if it's case 3 or 4
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, false, true);
	FACE oppFace = cube->getRelativeFace(FACE::BACK, "y", 2 + shifts);
	// OCLL3
	if ((cube->getFace(oppFace) & rowMask.first) == rowMask.second)
	{
		// adjust up face
		if (shifts == 1)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: R2 D (R' U2 R) D' (R' U2 R')
		cube->readMoves("R2 D R' U2 R D' R' U2 R'");
		std::cout << "R2 D R' U2 R D' R' U2 R'";
	}
	// OCLL4
	else
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP) << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP, "2") << " ";

		// perform OLL: (r U R' U') (r' F R F')
		cube->readMoves("(r U R' U') (r' F R F')");
		std::cout << "(r U R' U') (r' F R F')";
	}
}

/**
* Solve OLL case OCLL5
*/
void solveOCLL5(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// adjust up face
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, false, false);
	FACE face = cube->getRelativeFace(FACE::FRONT, "y", shifts);
	if ((cube->getFace(face) & rowMask.first) == rowMask.second)
	{
		if (shifts == 1)
			std::cout << cube->move(FACE::UP, "prime") << " ";
	}
	else
	{
		if (shifts == 0)
			std::cout << cube->move(FACE::UP, "2");
		else if (shifts == 1)
			std::cout << cube->move(FACE::UP);
	}
	// perform OLL: y F' (r U R' U') r' F R
	cube->readMoves("y F' (r U R' U') r' F R");
	std::cout << "y F' (r U R' U') r' F R";
}

/**
* Solve OLL case E1
*/
void solveE1(Cube* cube, uint8_t shifts)
{
	// adjust up face
	if (shifts == 1)
		std::cout << cube->move(FACE::UP, "prime") << " ";
	else if (shifts == 2)
		std::cout << cube->move(FACE::UP, "2") << " ";
	else if (shifts == 3)
		std::cout << cube->move(FACE::UP) << " ";

	// perform OLL: (r U R' U') M (U R U' R')
	cube->readMoves("(r U R' U') M (U R U' R')");
	std::cout << "(r U R' U') M (U R U' R')";
}

/**
* Solve OLL case E2
*/
void solveE2(Cube* cube, uint8_t shifts)
{
	// adjust up face
	if (shifts == 1)
		std::cout << cube->move(FACE::UP) << " ";

	// perform OLL: (R U R' U') M' (U R U' r')
	cube->readMoves("(R U R' U') M' (U R U' r')");
	std::cout << "(R U R' U') M' (U R U' r')";
}

/**
* Solve OLL cases OCLL6 and OCLL7
*/
void solveOCLL67(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// determine if it's case 6 or 7
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, false, false);
	FACE face = cube->getRelativeFace(FACE::BACK, "y", shifts);
	// OCLL6
	if ((cube->getFace(face) & rowMask.first) == rowMask.second)
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP) << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP, "2") << " ";

		// perform OLL: R U2 R' U' R U' R'
		cube->readMoves("R U2 R' U' R U' R'");
		std::cout << "R U2 R' U' R U' R'";
	}
	// OCLL7
	else
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 1)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: R U R' U R U2 R'
		cube->readMoves("R U R' U R U2 R'");
		std::cout << "R U R' U R U2 R'";
	}
}

/**
* Solve OLL cases OCLL1 and OCLL2
*/
void solveOCLL12(Cube* cube, COLOR topColor)
{
	// determine if it's case 1 or 2
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, false, true);
	FACE face = FACE::FRONT;
	// find the face with headlights
	while (true)
	{
		if ((cube->getFace(face) & rowMask.first) == rowMask.second)
			break;
		face = cube->getAdjacentFace(face, "y");
	}
	// check if opposite face also has headlights
	FACE oppFace = cube->getOppositeFace(face);
	// OCLL1
	if ((cube->getFace(oppFace) & rowMask.first) == rowMask.second)
	{
		// adjust up face
		if (face == FACE::RIGHT || face == FACE::LEFT)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: (R U2 R') (U' R U R') (U' R U' R')
		cube->readMoves("(R U2 R') (U' R U R') (U' R U' R')");
		std::cout << "(R U2 R') (U' R U R') (U' R U' R')";
	}
	// OCLL2
	else
	{
		// adjust up face
		if (face == FACE::BACK)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (face == FACE::RIGHT)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (face == FACE::FRONT)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: R U2 R2 U' R2 U' R2 U2 R
		cube->readMoves("R U2 R2 U' R2 U' R2 U2 R");
		std::cout << "R U2 R2 U' R2 U' R2 U2 R";
	}
}

/**
* Solve OLL cases T1 and T2
*/
void solveT12(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// adjust up face (same for both cases)
	if (shifts == 1)
		std::cout << cube->move(FACE::UP, "prime") << " ";
	else if (shifts == 2)
		std::cout << cube->move(FACE::UP, "2") << " ";
	else if (shifts == 3)
		std::cout << cube->move(FACE::UP) << " ";

	// determine if it's case 1 or 2
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, true, false);
	// T1
	if ((cube->getFace(FACE::FRONT) & rowMask.first) == rowMask.second)
	{
		// perform OLL: (R U R' U') (R' F R F')
		cube->readMoves("(R U R' U') (R' F R F')");
		std::cout << "(R U R' U') (R' F R F')";
	}
	// T2
	else
	{
		// perform OLL: F (R U R' U') F'
		cube->readMoves("F (R U R' U') F'");
		std::cout << "F (R U R' U') F'";
	}
}

/**
* Solve OLL cases C1 and C2
*/
void solveC12(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// determine if it's case 1 or 2
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, true, true);
	FACE face = cube->getRelativeFace(FACE::FRONT, "y", shifts);
	// C1
	if ((cube->getFace(face) & rowMask.first) != rowMask.second)
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 1)
			std::cout << cube->move(FACE::UP) << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP, "prime") << " ";

		// perform OLL: (R U R2' U') (R' F R U) R U' F'
		cube->readMoves("(R U R2' U') (R' F R U) R U' F'");
		std::cout << "(R U R2' U') (R' F R U) R U' F'";
	}
	// C2
	else
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 1)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: R' U' (R' F R F') U R
		cube->readMoves("R' U' (R' F R F') U R");
		std::cout << "R' U' (R' F R F') U R";
	}
}

/**
* Solve OLL cases W1 and W2
*/
void solveW12(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// determine if it's case 1 or 2
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, false, true, true);
	FACE face = cube->getRelativeFace(FACE::RIGHT, "y", shifts);
	// W1
	if ((cube->getFace(face) & rowMask.first) == rowMask.second)
	{
		// adjust up face
		if (shifts == 1)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: (R' U' R U') (R' U R U) l U' R' U x
		cube->readMoves("(R' U' R U') (R' U R U) l U' R' U x");
		std::cout << "(R' U' R U') (R' U R U) l U' R' U x";
	}
	// W2
	else
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP) << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP, "2") << " ";

		// perform OLL: (R U R' U) (R U' R' U') (R' F R F')
		cube->readMoves("(R U R' U) (R U' R' U') (R' F R F')");
		std::cout << "(R U R' U) (R U' R' U') (R' F R F')";
	}
}

/**
* Solve OLL cases P1 and P3
*/
void solveP13(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// determine if it's case 1 or 3
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, true, false);
	FACE face = cube->getRelativeFace(FACE::FRONT, "y", shifts);
	// P1
	if ((cube->getFace(face) & rowMask.first) == rowMask.second)
	{
		// adjust up face
		if (shifts == 1)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: (R' U' F) (U R U' R') F' R
		cube->readMoves("(R' U' F) (U R U' R') F' R");
		std::cout << "(R' U' F) (U R U' R') F' R";
	}
	// P3
	else
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 1)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: R' U' F' U F R
		cube->readMoves("R' U' F' U F R");
		std::cout << "R' U' F' U F R";
	}
}

/**
* Solve OLL cases P2 and P4
*/
void solveP24(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// adjust up face (same for both cases)
	if (shifts == 1)
		std::cout << cube->move(FACE::UP, "prime") << " ";
	else if (shifts == 2)
		std::cout << cube->move(FACE::UP, "2") << " ";
	else if (shifts == 3)
		std::cout << cube->move(FACE::UP) << " ";

	// determine if it's case 2 or 4
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, false, false);
	// P2
	if ((cube->getFace(FACE::FRONT) & rowMask.first) == rowMask.second)
	{
		// perform OLL: R U B' (U' R' U) (R B R')
		cube->readMoves("R U B' (U' R' U) (R B R')");
		std::cout << "R U B' (U' R' U) (R B R')";
	}
	// P4
	else
	{
		// perform OLL: f (R U R' U') f'
		cube->readMoves("f (R U R' U') f'");
		std::cout << "f (R U R' U') f'";
	}
}

/**
* Solve OLL cases F3 and F4
*/
void solveF34(Cube* cube, COLOR topColor, uint8_t shifts)
{
	// determine if it's case 3 or 4
	std::pair<uint64_t, uint64_t> rowMask = generateRowMask(topColor, true, true, false);
	FACE face = cube->getRelativeFace(FACE::FRONT, "y", shifts);
	// F3
	if ((cube->getFace(face) & rowMask.first) != rowMask.second)
	{
		// adjust up face
		if (shifts == 0)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 1)
			std::cout << cube->move(FACE::UP) << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP, "prime") << " ";

		// perform OLL: (R U2') (R2' F R F') (R U2' R')
		cube->readMoves("(R U2') (R2' F R F') (R U2' R')");
		std::cout << "(R U2') (R2' F R F') (R U2' R')";
	}
	// F4
	else
	{
		// adjust up face
		if (shifts == 1)
			std::cout << cube->move(FACE::UP, "prime") << " ";
		else if (shifts == 2)
			std::cout << cube->move(FACE::UP, "2") << " ";
		else if (shifts == 3)
			std::cout << cube->move(FACE::UP) << " ";

		// perform OLL: F (R U' R' U') (R U R' F')
		cube->readMoves("F (R U' R' U') (R U R' F')");
		std::cout << "F (R U' R' U') (R U R' F')";
	}
}

/**
* Orient the last layer on the given cube.
*
* Assumes the first two layers are solved and that
* the cross color is oriented down.
*/
void solveOLL(Cube* cube)
{
	// get the color of the top face
	COLOR topColor = cube->getCenter(FACE::UP);
	// find the OLL type
	std::pair<uint8_t, uint8_t> ollType = findOLLType(cube, topColor);

	std::cout << "OLL solution:" << std::endl;

	// solve OLL based on the type
	if (ollType.first == olls[0])
		solveOCLL34(cube, topColor, ollType.second);
	else if (ollType.first == olls[1])
		solveOCLL5(cube, topColor, ollType.second);
	else if (ollType.first == olls[2])
		solveE1(cube, ollType.second);
	else if (ollType.first == olls[3])
		solveE2(cube, ollType.second);
	else if (ollType.first == olls[4])
		solveOCLL67(cube, topColor, ollType.second);
	else if (ollType.first == olls[5])
		solveOCLL12(cube, topColor);
	else if (ollType.first == olls[6])
		solveT12(cube, topColor, ollType.second);
	else if (ollType.first == olls[7])
		solveC12(cube, topColor, ollType.second);
	else if (ollType.first == olls[8])
		solveW12(cube, topColor, ollType.second);
	else if (ollType.first == olls[9])
		solveP13(cube, topColor, ollType.second);
	else if (ollType.first == olls[10])
		solveP24(cube, topColor, ollType.second);
	else if (ollType.first == olls[11])
		solveF34(cube, topColor, ollType.second);

	std::cout << "\n\n";
}
