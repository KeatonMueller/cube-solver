#include <iostream>

#include "SolverCross.h"

/**
* Return the location of an unsolved edge piece of the given color.
* Second item in pair is false if no more unsolved edge pieces remain.
*/
std::pair<LOCATION, bool> findUnsolvedCrossEdge(Cube* cube, COLOR color)
{
	// search every sticker on every face
	for (uint8_t face = 0; face < 6; face++)
	{
		for (uint8_t idx = 1; idx < 8; idx++)
		{
			// skip even indices - they are corner pieces
			if (idx % 2 == 0)
				continue;
			if (cube->getSticker({ (FACE)face, idx }) == color && !cube->isPieceSolved({ (FACE)face, idx }))
				return std::make_pair(LOCATION({ (FACE)face, idx }), true);
		}
	}
	return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
}

/**
* Perform the moves necessary to bring the given piece
* location into the top layer without disturbing any possible
* cross piece that is already solved.
*
* Cross color is not necessarily oriented facing up after this step.
* Cross color center is assumed to be facing down.
*
* Return the piece's new location.
*/
LOCATION bringEdgeToTopLayer(Cube* cube, LOCATION piece)
{
	// find which layer it currently is
	LAYER layer = getLayer(cube, piece);

	// perform moves necessary to bring to top
	if (layer == LAYER::TOP)
		return piece;

	else if (layer == LAYER::BOTTOM)
	{
		// piece is in bottom layer not facing the bottom
		if (piece.face != FACE::DOWN)
		{
			// twist its face twice
			std::cout << cube->move(piece.face, "2") << " ";
			return { piece.face, 1 };
		}
		// piece is in bottom layer facing the bottom
		else
		{
			// twist the adjacent face twice
			std::cout << cube->move(cube->getAdjacentEdge(piece).first.face, "2") << " ";
			uint8_t newIdx = (piece.idx == 3 || piece.idx == 7) ? piece.idx : 6 - piece.idx;
			return { FACE::UP, newIdx };
		}
	}
	else
	{
		// piece is somewhere in the middle
		LOCATION adj = cube->getAdjacentEdge(piece).first;
		// bring it to the top such that white is facing up
		if (adj.idx == 3)
		{
			std::cout << cube->move(adj.face, "prime") << " ";
			std::cout << cube->move(FACE::UP) << " ";
			std::cout << cube->move(adj.face) << " ";
		}
		else if (adj.idx == 7)
		{
			std::cout << cube->move(adj.face) << " ";
			std::cout << cube->move(FACE::UP) << " ";
			std::cout << cube->move(adj.face, "prime") << " ";
		}
		// determine the new position of the piece
		uint8_t newIdx = 0;
		switch (adj.face)
		{
		case FACE::FRONT:
			newIdx = 7;
			break;
		case FACE::BACK:
			newIdx = 3;
			break;
		case FACE::RIGHT:
			newIdx = 5;
			break;
		case FACE::LEFT:
			newIdx = 1;
			break;
		}
		return { FACE::UP, newIdx };
	}
}

/**
* Move the given edge piece so that it's adjacent sticker
* lines up with the center piece.
*
* Assumes that the given location is in the top layer.
*
* Return the piece's new location.
*/
LOCATION moveEdgeOverCenter(Cube* cube, LOCATION piece)
{
	uint8_t moves = 0;
	// if cross color is facing up
	if (piece.face == FACE::UP)
	{
		LOCATION adj = cube->getAdjacentEdge(piece).first;

		// turn the up face until the edge sticker is aligned
		// could optimize this so U3 isn't possible in the future
		while (cube->getSticker(adj) != cube->getCenter(adj.face))
		{
			//std::cout << cube->move(FACE::UP) << " ";
			cube->u();
			piece.idx = (piece.idx + 2) % 8;
			adj = cube->getAdjacentEdge(piece).first;
			moves++;
		}
	}
	// if cross color is facing out
	else
	{
		COLOR targetColor = cube->getSticker(cube->getAdjacentEdge(piece).first);
		while (targetColor != cube->getCenter(piece.face))
		{
			//std::cout << cube->move(FACE::UP) << " ";
			cube->u();
			switch (piece.face)
			{
			case FACE::FRONT:
				piece.face = FACE::LEFT;
				break;
			case FACE::LEFT:
				piece.face = FACE::BACK;
				break;
			case FACE::BACK:
				piece.face = FACE::RIGHT;
				break;
			case FACE::RIGHT:
				piece.face = FACE::FRONT;
				break;
			}
			moves++;
		}
	}
	// print instructions based on how many moves it took
	if (moves == 1)
		std::cout << "U ";
	else if (moves == 2)
		std::cout << "U2 ";
	else if (moves == 3)
		std::cout << "U\' ";

	// return the piece location
	return piece;
}

/**
* Insert the cross piece at the given location into
* the cross. The cross piece is not necessarily facing
* up, so there are two possible ways to insert it.
*
* Assumes that the piece is located in the top layer directly
* over the location it needs to be inserted into.
*/
void insertCrossEdge(Cube* cube, LOCATION piece)
{
	// simple case is when white is facing up
	if (piece.face == FACE::UP)
	{
		// turn adjacent side twice
		std::cout << cube->move(cube->getAdjacentEdge(piece).first.face, "2") << "\n";
	}
	// white is facing to the side
	else
	{
		// turn the up face counter clockwise
		std::cout << cube->move(FACE::UP, "prime") << " ";
		// turn the face to the right counter clockwise
		FACE adjFace = cube->getAdjacentEdge({ piece.face, 3 }).first.face;
		std::cout << cube->move(adjFace, "prime") << " ";
		// turn piece's face clockwise
		std::cout << cube->move(piece.face) << " ";
		// undo the adjacent face's turn
		std::cout << cube->move(adjFace) << "\n";
	}
}

/**
* Solve the cross piece at the given location.
* Assumes that the location points to the sticker of the cross
* color, and that the cross color's center is already facing
* down.
*/
void solveCrossPiece(Cube* cube, LOCATION piece)
{
	if (cube->isPieceSolved(piece))
		return;

	piece = bringEdgeToTopLayer(cube, piece);
	piece = moveEdgeOverCenter(cube, piece);
	insertCrossEdge(cube, piece);
}

/**
* Orient the cube so the given color is facing down.
*/
void orientDown(Cube* cube, COLOR color)
{
	if (cube->getCenter(FACE::UP) == color)
	{
		std::cout << "Z2\n";
		cube->z();
		cube->z();
	}
	else if (cube->getCenter(FACE::DOWN) == color)
	{
		return;
	}
	else if (cube->getCenter(FACE::FRONT) == color)
	{
		std::cout << "X\'\n";
		cube->xPrime();
	}
	else if (cube->getCenter(FACE::BACK) == color)
	{
		std::cout << "X\n";
		cube->x();
	}
	else if (cube->getCenter(FACE::RIGHT) == color)
	{
		std::cout << "Z\n";
		cube->z();
	}
	else if (cube->getCenter(FACE::LEFT) == color)
	{
		std::cout << "Z\'\n";
		cube->zPrime();
	}
}


/**
* Solve the cross of the given color on the given cube.
*
* Does not assume that any part of the cube is solved, nor
* does it assume an orientation of the cube.
*/
void solveCross(Cube* cube, COLOR color)
{
	std::cout << "Cross solution:" << std::endl;
	// orient the cube so the cross color is on the bottom
	orientDown(cube, color);

	// solve cross
	std::pair<LOCATION, bool> edgeLoc = findUnsolvedCrossEdge(cube, color);
	while (edgeLoc.second)
	{
		solveCrossPiece(cube, edgeLoc.first);
		edgeLoc = findUnsolvedCrossEdge(cube, color);
	}
	std::cout << "\n\n";
}
