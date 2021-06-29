#include <iostream>

#include "Solver2L.h"

/**
* Find an unsolved second layer edge on the given cube.
*
* The provided color is the edge color to *avoid*, so the
* returned location will be to an edge that does not have that color.
*
* If no unsolved edge remains, the second item in the pair will be false,
* otherwise it is true.
*/
std::pair<LOCATION, bool> findUnsolved2LEdge(Cube* cube, COLOR color)
{
	// search every sticker on every face
	// NOTE: it's important here that the search starts with FACE::UP (0)
	//       in order to avoid unnecessary work
	for (uint8_t face = 0; face < 6; face++)
	{
		for (uint8_t idx = 1; idx < 8; idx++)
		{
			// skip even indices - they are corner pieces
			if (idx % 2 == 0)
				continue;
			// piece must not have provided color, and it must be unsolved
			if (cube->getSticker({ (FACE)face, idx }) != color
				&& cube->getSticker(cube->getAdjacentEdge({ (FACE)face, idx }).first) != color
				&& !cube->isEdgeSolved({ (FACE)face, idx }))
				return std::make_pair(LOCATION({ (FACE)face, idx }), true);
		}
	}
	// no unsolved second layer edge found
	return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
}

/**
* Bring the given edge piece into the top layer without
* disturbing the first layer or any solved second layer
* edges.
*/
LOCATION bring2LEdgeToTopLayer(Cube* cube, LOCATION piece)
{
	// find which layer it currently is
	LAYER layer = getLayer(cube, piece);

	// perform moves necessary to bring to top
	if (layer == LAYER::TOP)
		return piece;
	else if (layer == LAYER::MIDDLE)
	{
		// identify both stickers
		FACE leftFace = piece.idx == 3 ? piece.face : cube->getAdjacentEdge(piece).first.face;
		FACE rightFace = piece.idx == 7 ? piece.face : cube->getAdjacentEdge(piece).first.face;

		// right sexy move
		std::cout << cube->move(rightFace) << " ";
		std::cout << cube->move(FACE::UP) << " ";
		std::cout << cube->move(rightFace, "prime") << " ";
		std::cout << cube->move(FACE::UP, "prime") << " ";
		// left sexy move (mostly)
		std::cout << cube->move(leftFace, "prime") << " ";
		std::cout << cube->move(FACE::UP, "prime") << " ";
		std::cout << cube->move(leftFace) << " ";

		// location is now idx 1 of face opposite leftFace
		piece.face = cube->getOppositeFace(leftFace);
		piece.idx = 1;
	}
	else
	{
		// not possible for it to be in the bottom layer
		// because first layer is presumed to be solved
	}
	return piece;
}

/**
* Align the outward facing sticker on the edge piece with
* its corresponding center.
*/
LOCATION align2LEdge(Cube* cube, LOCATION piece)
{
	// ensure we're working with the outer edge
	COLOR toMatch = piece.face != FACE::UP ? cube->getSticker(piece) : cube->getSticker(cube->getAdjacentEdge(piece).first);
	FACE currFace = piece.face != FACE::UP ? piece.face : cube->getAdjacentEdge(piece).first.face;

	uint8_t moves = 0;
	while (toMatch != cube->getCenter(currFace))
	{
		cube->u();
		currFace = cube->getAdjacentFace(currFace, "y");
		moves++;
	}
	piece.idx = 1;
	piece.face = currFace;

	if (moves == 1)
		std::cout << "U ";
	else if (moves == 2)
		std::cout << "U2 ";
	else if (moves == 3)
		std::cout << "U\' ";

	return piece;
}

/**
* Insert the second layer edge into the proper position.
*
* Assumes the given location is in the top layer.
*/
void insert2LEdge(Cube* cube, LOCATION piece)
{
	// get both stickers
	LOCATION topPiece = piece.face == FACE::UP ? piece : cube->getAdjacentEdge(piece).first;
	LOCATION sidePiece = piece.face != FACE::UP ? piece : cube->getAdjacentEdge(piece).first;

	// determine if inserting to the right or to the left
	FACE leftFace = cube->getAdjacentFace(sidePiece.face, "y");
	FACE rightFace = cube->getAdjacentFace(sidePiece.face, "yPrime");

	// insert to the right
	if (cube->getCenter(rightFace) == cube->getSticker(topPiece))
	{
		std::cout << cube->move(FACE::UP) << " ";
		std::cout << cube->move(rightFace) << " ";
		std::cout << cube->move(FACE::UP, "prime") << " ";
		std::cout << cube->move(rightFace, "prime") << " ";
		std::cout << cube->move(FACE::UP, "prime") << " ";
		std::cout << cube->move(sidePiece.face, "prime") << " ";
		std::cout << cube->move(FACE::UP) << " ";
		std::cout << cube->move(sidePiece.face) << " ";
	}
	// insert to the left
	else if (cube->getCenter(leftFace) == cube->getSticker(topPiece))
	{
		std::cout << cube->move(FACE::UP, "prime") << " ";
		std::cout << cube->move(leftFace, "prime") << " ";
		std::cout << cube->move(FACE::UP) << " ";
		std::cout << cube->move(leftFace) << " ";
		std::cout << cube->move(FACE::UP) << " ";
		std::cout << cube->move(sidePiece.face) << " ";
		std::cout << cube->move(FACE::UP, "prime") << " ";
		std::cout << cube->move(sidePiece.face, "prime") << " ";
	}
}

/**
* Solve the given second layer edge
*/
void solveSecondLayerEdge(Cube* cube, LOCATION piece)
{
	if (cube->isEdgeSolved(piece))
		return;

	piece = bring2LEdgeToTopLayer(cube, piece);
	piece = align2LEdge(cube, piece);
	insert2LEdge(cube, piece);
}

/**
* Solve the second layer of the given cube.
*
* Cross color is assumed to be oriented down,
* and the entire first layer is assumed to be solved
*/
void solveSecondLayer(Cube* cube)
{
	// the edge color to avoid is the up face's color
	COLOR color = cube->getCenter(FACE::UP);

	std::cout << "Second layer solution:" << std::endl;

	// solve corners
	std::pair<LOCATION, bool> edgeLoc = findUnsolved2LEdge(cube, color);
	while (edgeLoc.second)
	{
		solveSecondLayerEdge(cube, edgeLoc.first);
		edgeLoc = findUnsolved2LEdge(cube, color);
	}
	std::cout << "\n\n";
}