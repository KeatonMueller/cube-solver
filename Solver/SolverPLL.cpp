#include <iostream>

#include "SolverPLL.h"
#include "Util.h"

/**
* Total number of PLLs.
*/
const uint8_t NUM_PLLS = 21;

/**
* Values corresponding to every single PLL case.
*
* The 64-bit integer is laid out the same way cube faces
* are in Cube.h, but instead of storing sticker values,
* they store location values.
*
* The index of where a particular piece is supposed to go
* is stored in each position.
*
* The name of each permutation is commented on the same line.
*
* The algorithms used in this solver come from this resource:
* https://www.cubeskills.com/uploads/pdf/tutorials/pll-algorithms.pdf
*/
uint64_t plls[NUM_PLLS] = {
	((uint64_t)0 << 56) | ((uint64_t)1 << 48) | ((uint64_t)2 << 40) | ((uint64_t)7 << 32) | ((uint64_t)4 << 24) | ((uint64_t)3 << 16) | ((uint64_t)6 << 8) | ((uint64_t)5 << 0), // Ua
	((uint64_t)0 << 56) | ((uint64_t)1 << 48) | ((uint64_t)2 << 40) | ((uint64_t)5 << 32) | ((uint64_t)4 << 24) | ((uint64_t)7 << 16) | ((uint64_t)6 << 8) | ((uint64_t)3 << 0), // Ub
	((uint64_t)0 << 56) | ((uint64_t)7 << 48) | ((uint64_t)2 << 40) | ((uint64_t)5 << 32) | ((uint64_t)4 << 24) | ((uint64_t)3 << 16) | ((uint64_t)6 << 8) | ((uint64_t)1 << 0), // Z
	((uint64_t)0 << 56) | ((uint64_t)5 << 48) | ((uint64_t)2 << 40) | ((uint64_t)7 << 32) | ((uint64_t)4 << 24) | ((uint64_t)1 << 16) | ((uint64_t)6 << 8) | ((uint64_t)3 << 0), // H
	((uint64_t)2 << 56) | ((uint64_t)1 << 48) | ((uint64_t)4 << 40) | ((uint64_t)3 << 32) | ((uint64_t)0 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)7 << 0), // Aa
	((uint64_t)4 << 56) | ((uint64_t)1 << 48) | ((uint64_t)0 << 40) | ((uint64_t)3 << 32) | ((uint64_t)2 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)7 << 0), // Ab
	((uint64_t)6 << 56) | ((uint64_t)1 << 48) | ((uint64_t)4 << 40) | ((uint64_t)3 << 32) | ((uint64_t)2 << 24) | ((uint64_t)5 << 16) | ((uint64_t)0 << 8) | ((uint64_t)7 << 0), // E
	((uint64_t)0 << 56) | ((uint64_t)7 << 48) | ((uint64_t)4 << 40) | ((uint64_t)3 << 32) | ((uint64_t)2 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)1 << 0), // Ra
	((uint64_t)2 << 56) | ((uint64_t)1 << 48) | ((uint64_t)0 << 40) | ((uint64_t)5 << 32) | ((uint64_t)4 << 24) | ((uint64_t)3 << 16) | ((uint64_t)6 << 8) | ((uint64_t)7 << 0), // Rb
	((uint64_t)2 << 56) | ((uint64_t)7 << 48) | ((uint64_t)0 << 40) | ((uint64_t)3 << 32) | ((uint64_t)4 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)1 << 0), // Ja
	((uint64_t)0 << 56) | ((uint64_t)1 << 48) | ((uint64_t)4 << 40) | ((uint64_t)5 << 32) | ((uint64_t)2 << 24) | ((uint64_t)3 << 16) | ((uint64_t)6 << 8) | ((uint64_t)7 << 0), // Jb
	((uint64_t)0 << 56) | ((uint64_t)1 << 48) | ((uint64_t)4 << 40) | ((uint64_t)7 << 32) | ((uint64_t)2 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)3 << 0), // T
	((uint64_t)0 << 56) | ((uint64_t)5 << 48) | ((uint64_t)4 << 40) | ((uint64_t)3 << 32) | ((uint64_t)2 << 24) | ((uint64_t)1 << 16) | ((uint64_t)6 << 8) | ((uint64_t)7 << 0), // F
	((uint64_t)4 << 56) | ((uint64_t)3 << 48) | ((uint64_t)2 << 40) | ((uint64_t)1 << 32) | ((uint64_t)0 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)7 << 0), // V
	((uint64_t)4 << 56) | ((uint64_t)7 << 48) | ((uint64_t)2 << 40) | ((uint64_t)3 << 32) | ((uint64_t)0 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)1 << 0), // Y
	((uint64_t)0 << 56) | ((uint64_t)1 << 48) | ((uint64_t)6 << 40) | ((uint64_t)7 << 32) | ((uint64_t)4 << 24) | ((uint64_t)5 << 16) | ((uint64_t)2 << 8) | ((uint64_t)3 << 0), // Na
	((uint64_t)4 << 56) | ((uint64_t)1 << 48) | ((uint64_t)2 << 40) | ((uint64_t)7 << 32) | ((uint64_t)0 << 24) | ((uint64_t)5 << 16) | ((uint64_t)6 << 8) | ((uint64_t)3 << 0), // Nb
	((uint64_t)2 << 56) | ((uint64_t)7 << 48) | ((uint64_t)6 << 40) | ((uint64_t)1 << 32) | ((uint64_t)4 << 24) | ((uint64_t)5 << 16) | ((uint64_t)0 << 8) | ((uint64_t)3 << 0), // Ga
	((uint64_t)6 << 56) | ((uint64_t)3 << 48) | ((uint64_t)0 << 40) | ((uint64_t)7 << 32) | ((uint64_t)4 << 24) | ((uint64_t)5 << 16) | ((uint64_t)2 << 8) | ((uint64_t)1 << 0), // Gb
	((uint64_t)6 << 56) | ((uint64_t)1 << 48) | ((uint64_t)2 << 40) | ((uint64_t)5 << 32) | ((uint64_t)0 << 24) | ((uint64_t)7 << 16) | ((uint64_t)4 << 8) | ((uint64_t)3 << 0), // Gc
	((uint64_t)2 << 56) | ((uint64_t)7 << 48) | ((uint64_t)6 << 40) | ((uint64_t)3 << 32) | ((uint64_t)4 << 24) | ((uint64_t)1 << 16) | ((uint64_t)0 << 8) | ((uint64_t)5 << 0), // Gd
};

/**
* Strings storing every single PLL algorithm.
*
* The name of each algorithm is commented on each line, and the
* order matches the plls array above.
*/
std::string pllAlgs[NUM_PLLS] = {
	"(R U' R U) R U (R U' R' U') R2",								   // Ua
	"R2 U (R U R' U') R' U' (R' U R')",								   // Ub
	"(M2' U M2' U) (M' U2) (M2' U2 M')",							   // Z
	"(M2' U M2') U2 (M2' U M2')",									   // H
	"x (R' U R') D2 (R U' R') D2 R2 x'",							   // Aa
	"x R2' D2 (R U R') D2 (R U' R) x'",								   // Ab
	"x' (R U' R' D) (R U R' D') (R U R' D) (R U' R' D') x",			   // E
	"(R U' R' U') (R U R D) (R' U' R D') (R' U2 R')",				   // Ra
	"(R' U2 R U2') R' F (R U R' U') R' F' R2",						   // Rb
	"(R' U L' U2) (R U' R' U2 R) L",								   // Ja
	"(R U R' F') (R U R' U') R' F R2 U' R'",						   // Jb
	"(R U R' U') (R' F R2 U') R' U' (R U R' F')",					   // T
	"(R' U' F') (R U R' U') (R' F R2 U') (R' U' R U) (R' U R)",		   // F
	"(R' U R' U') y (R' F' R2 U') (R' U R' F) R F",					   // V
	"F (R U' R' U') (R U R' F') (R U R' U') (R' F R F')",			   // Y
	"(R U R' U) (R U R' F') (R U R' U') (R' F R2 U') R' U2 (R U' R')", // Na
	"(R' U R U') (R' F' U' F) (R U R' F) R' F' (R U' R)",			   // Nb
	"R2 U (R' U R' U') (R U' R2) D U' (R' U R D')",					   // Ga
	"(F' U' F) (R2 u R' U) (R U' R u') R2'",						   // Gb
	"R2 U' (R U' R U) (R' U R2 D') (U R U' R') D",					   // Gc
	"D' (R U R' U') D (R2 U' R U') (R' U R' U) R2"					   // Gd
};

/**
* Return the index into the faces array for the given face.
*
* Only FRONT, BACK, RIGHT, and LEFT are valid, all other faces
* return an out of bounds index of 4.
*/
uint8_t getFaceIdx(FACE face)
{
	switch (face)
	{
	case FACE::FRONT:
		return 0;
	case FACE::BACK:
		return 1;
	case FACE::RIGHT:
		return 2;
	case FACE::LEFT:
		return 3;
	default:
		return 4;
	}
}

/**
* Check if the piece at the given location belongs
* in the given face.
*/
bool checkLocation(Cube& cube, COLOR(&faces)[4], FACE face, LOCATION loc)
{
	uint8_t idx = getFaceIdx(face);
	if (faces[idx] == COLOR::EMPTY)
		faces[idx] = cube.getSticker(loc);
	else if (faces[idx] != cube.getSticker(loc))
		return false;
	return true;
}

/**
* Determine if the piece in the top layer at currIdx should move
* to targetIdx on the given cube.
*/
bool shouldMoveTo(Cube& cube, COLOR(&faces)[4], uint8_t currIdx, uint8_t targetIdx)
{
	// if moving a corner piece
	if (currIdx % 2 == 0)
	{
		// get the adjacent locations
		std::pair<LOCATION, LOCATION> adjCurrent = cube.getAdjacentCorner({ FACE::UP, currIdx });

		// if move is to opposite corner
		if ((currIdx + 4) % 8 == targetIdx || (currIdx - 4) % 8 == targetIdx)
		{
			// each sticker must be able to go to opposite face
			FACE oppFace0 = cube.getOppositeFace(adjCurrent.first.face);
			FACE oppFace1 = cube.getOppositeFace(adjCurrent.second.face);

			if (!checkLocation(cube, faces, oppFace0, adjCurrent.first))
				return false;
			if (!checkLocation(cube, faces, oppFace1, adjCurrent.second))
				return false;
		}
		// if move is to adjacent corner
		else
		{
			// clockwise move
			if ((currIdx + 2) % 8 == targetIdx)
			{
				if (currIdx == 0 || currIdx == 4)
				{
					// first adjacent goes to the face opposite of second adjacent
					if (!checkLocation(cube, faces, cube.getOppositeFace(adjCurrent.second.face), adjCurrent.first))
						return false;
					// second adjacent goes to the face of the first adjacent
					if (!checkLocation(cube, faces, adjCurrent.first.face, adjCurrent.second))
						return false;
				}
				else if (currIdx == 2 || currIdx == 6)
				{
					// first adjacent goes to the face of second adjacent
					if (!checkLocation(cube, faces, adjCurrent.second.face, adjCurrent.first))
						return false;
					// second adjacent goes to the face opposite of first adjacent
					if (!checkLocation(cube, faces, cube.getOppositeFace(adjCurrent.first.face), adjCurrent.second))
						return false;
				}
			}
			// counter clockwise move
			else if ((currIdx - 2) % 8 == targetIdx)
			{
				if (currIdx == 0 || currIdx == 4)
				{
					// first adjacent goes to the face of second adjacent
					if (!checkLocation(cube, faces, adjCurrent.second.face, adjCurrent.first))
						return false;
					// second adjacent goes to the face opposite of first adjacent
					if (!checkLocation(cube, faces, cube.getOppositeFace(adjCurrent.first.face), adjCurrent.second))
						return false;
				}
				else if (currIdx == 2 || currIdx == 6)
				{
					// first adjacent goes to the face opposite of second adjacent
					if (!checkLocation(cube, faces, cube.getOppositeFace(adjCurrent.second.face), adjCurrent.first))
						return false;
					// second adjacent goes to the face of the first adjacent
					if (!checkLocation(cube, faces, adjCurrent.first.face, adjCurrent.second))
						return false;
				}
			}
		}
	}
	// if moving an edge piece
	else
	{
		// make sure the target face matches the current sticker
		FACE targetFace = cube.getAdjacentEdge({ FACE::UP, targetIdx }).face;
		LOCATION adj = cube.getAdjacentEdge({ FACE::UP, currIdx });
		if (!checkLocation(cube, faces, targetFace, adj))
			return false;
	}
	return true;
}

/**
* Check if the piece at the given index is allowed to stay in
* its current position based on the faces array.
*/
bool canPieceStay(Cube& cube, COLOR(&faces)[4], uint8_t idx)
{
	// corner piece
	if (idx % 2 == 0)
	{
		std::pair<LOCATION, LOCATION> adjs = cube.getAdjacentCorner({ FACE::UP, idx });
		if (!checkLocation(cube, faces, adjs.first.face, adjs.first))
			return false;
		if (!checkLocation(cube, faces, adjs.second.face, adjs.second))
			return false;
		return true;
	}
	// edge piece
	else
	{
		LOCATION adj = cube.getAdjacentEdge({ FACE::UP, idx });
		if (!checkLocation(cube, faces, adj.face, adj))
			return false;
		return true;
	}
}

/**
* Determine if the given PLL exactly matches the current
* cube state, without any rotations needed.
*/
bool pllCaseMatches(Cube& cube, uint64_t pll)
{
	uint8_t targetIdx;
	COLOR faces[4] = { COLOR::EMPTY, COLOR::EMPTY, COLOR::EMPTY, COLOR::EMPTY };
	// for each piece location
	for (uint8_t currIdx = 0; currIdx < 8; currIdx++)
	{
		// get where the PLL says it should go
		targetIdx = (uint8_t)(pll >> ((7 - currIdx) * 8));
		// if targetIdx == currIdx, the piece is supposed to stay
		if (targetIdx == currIdx)
		{
			if (!canPieceStay(cube, faces, currIdx))
				return false;
		}
		// otherwise ensure it would be solved at the target
		else
		{
			if (!shouldMoveTo(cube, faces, currIdx, targetIdx))
				return false;
		}
	}
	return true;
}

/**
* Determine if the given PLL fits the cube state.
*
* Returns a (boolean, uint8_t) pair where the boolean indicates
* whether or not the given OLL matches the cube, and the integer
* indicates the number of clockwise rotations needed for the PLL
* to align with the cube state.
*/
std::pair<bool, uint8_t> pllCaseFits(Cube& cube, uint64_t pll)
{
	// check all four orientations of the PLL
	for (uint8_t shifts = 0; shifts < 4; shifts++)
	{
		// return how many shifts needed if PLL case matches
		if (pllCaseMatches(cube, pll))
			return std::make_pair(true, shifts);
		// shift the PLL
		pll = rotateRight(pll, 16);
	}
	// no orientation worked, return false in first element of pair
	return std::make_pair(false, 0);
}

/**
* Find which PLL case this is.
*
* Returns a pair of the PLL's index followed by the number of clockwise rotations
* needed for the PLL to match the cube state.
*/
std::pair<uint8_t, uint8_t> findPLLType(Cube& cube)
{
	for (uint8_t idx = 0; idx < NUM_PLLS; idx++)
	{
		std::pair<bool, uint8_t> pllShifts = pllCaseFits(cube, plls[idx]);
		if (pllShifts.first)
			return std::make_pair(idx, pllShifts.second);
	}
	// not possible since every PLL case is covered by the above for loop
	return std::make_pair(NUM_PLLS, 0);
}

/**
* Permute the last layer on the given cube.
*
* Assumes that first two layers are solved and that
* the last layer has been oriented.
*/
void solvePLL(Cube& cube, std::vector<Move>& solution)
{
	std::pair<uint8_t, uint8_t> pll = findPLLType(cube);

	// adjust up face
	if (pll.second == 1)
		solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
	else if (pll.second == 2)
		solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
	else if (pll.second == 3)
		solution.push_back(cube.move(FACE::UP));

	// perform PLL (if needed)
	if (pll.first != NUM_PLLS)
	{
		std::vector<Move> moves = cube.readMoves(pllAlgs[pll.first]);
		solution.insert(solution.end(), moves.begin(), moves.end());
	}

	// adjust up face
	uint8_t numTurns = 0;
	while (!cube.isSolved())
	{
		cube.u();
		numTurns++;
	}
	if (numTurns == 1)
		solution.push_back(Move(Move::PIECES::UP, Move::TYPE::NORMAL));
	else if (numTurns == 2)
		solution.push_back(Move(Move::PIECES::UP, Move::TYPE::DOUBLE));
	else if (numTurns == 3)
		solution.push_back(Move(Move::PIECES::UP, Move::TYPE::PRIME));

	solution.push_back(Move(Move::PIECES::Y, Move::TYPE::NO_MOVE));
}
