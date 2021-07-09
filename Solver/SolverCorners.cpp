#include <iostream>

#include "SolverCorners.h"

/**
* Return the location of an unsolved corner piece of the given color.
* Second item in pair is false if no more unsolved edge pieces remain.
*/
std::pair<LOCATION, bool> findUnsolvedCorner(Cube& cube, COLOR color)
{
	// search every sticker on every face
	for (uint8_t face = 0; face < 6; face++)
	{
		for (uint8_t idx = 0; idx < 7; idx++)
		{
			// skip odd indices - they are edge pieces
			if (idx % 2 == 1)
				continue;
			if (cube.getSticker({ (FACE)face, idx }) == color && !cube.isPieceSolved({ (FACE)face, idx }))
				return std::make_pair(LOCATION({ (FACE)face, idx }), true);
		}
	}
	return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
}

/**
* Bring the given corner piece into the top layer while
* not disturbing the cross or any other corners.
*/
LOCATION bringCornerToTop(Cube& cube, LOCATION piece, std::vector<Move>& solution)
{
	// find which layer it currently is
	LAYER layer = getLayer(piece);

	// perform moves necessary to bring to top
	if (layer == LAYER::TOP)
		return piece;

	else if (layer == LAYER::BOTTOM)
	{
		if (piece.face == FACE::DOWN)
		{
			// white sticker is facing down, but it's in the wrong slot
			std::pair<LOCATION, LOCATION> adjs = cube.getAdjacentCorner(piece);
			LOCATION adj = adjs.first.idx == 6 ? adjs.first : adjs.second;

			// turn the adjacent face clockwise
			solution.push_back(cube.move(adj.face));
			// turn up face counter clockwise
			solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
			// turn the adjacent face counter clockwise
			solution.push_back(cube.move(adj.face, Move::TYPE::PRIME));
			// update new location
			piece.face = adj.face;
			piece.idx = 0;
		}
		else
		{
			// the white sticker is facing to the side
			if (piece.idx == 4)
			{
				// turn face counter clockwise
				solution.push_back(cube.move(piece.face, Move::TYPE::PRIME));
				// turn up face counter clockwise
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
				// turn face clockwise
				solution.push_back(cube.move(piece.face));
				// find piece's new location
				std::pair<LOCATION, LOCATION> adj = cube.getAdjacentCorner(piece);
				piece.face = adj.second.face;
				piece.idx = 2;
			}
			else if (piece.idx == 6)
			{
				// turn face clockwise
				solution.push_back(cube.move(piece.face));
				// turn up face clockwise
				solution.push_back(cube.move(FACE::UP));
				// turn face counter clockwise
				solution.push_back(cube.move(piece.face, Move::TYPE::PRIME));
				// find piece's new location
				std::pair<LOCATION, LOCATION> adj = cube.getAdjacentCorner(piece);
				piece.face = adj.second.face;
				piece.idx = 0;
			}
		}
	}
	else
	{
		// not possible for a corner piece to be in the middle layer
	}

	return piece;
}

/**
* Determine if the given corner piece is currently positioned
* over the slot it needs to be inserted into.
*/
bool isCornerLocatedOverCenter(Cube& cube, LOCATION piece)
{
	// get the adjacent sides
	std::pair<LOCATION, LOCATION> adjs = cube.getAdjacentCorner(piece);
	// find the centers its located over
	if (piece.face == FACE::UP)
	{
		// if cross color is facing up, then opposite centers should match the adjacent stickers
		if (cube.getCenter(adjs.first.face) == cube.getSticker(adjs.second)
			&& cube.getCenter(adjs.second.face) == cube.getSticker(adjs.first))
			return true;
	}
	else
	{
		// otherwise, the piece facing up (always adjs.first)
		// should match the cross color's center
		if (cube.getCenter(piece.face) == cube.getSticker(adjs.first)
			&& cube.getCenter(adjs.second.face) == cube.getSticker(adjs.second))
			return true;
	}
	return false;
}

/**
* Rotate the up face until the given corner piece
* is positioned over the slot it needs to be inserted into.
*
* Assumes that the given location is a corner piece in the top
* layer.
*/
LOCATION moveCornerOverCenter(Cube& cube, LOCATION piece, std::vector<Move>& solution)
{
	uint8_t moves = 0;
	while (!isCornerLocatedOverCenter(cube, piece))
	{
		cube.u();
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
		case FACE::UP:
			piece.idx = (piece.idx + 2) % 8;
			break;
		default:
			break;
		}
		moves++;
	}
	// print instructions based on how many moves it took
	if (moves == 1)
		solution.push_back(Move(Move::PIECES::UP, Move::TYPE::NORMAL));
	else if (moves == 2)
		solution.push_back(Move(Move::PIECES::UP, Move::TYPE::DOUBLE));
	else if (moves == 3)
		solution.push_back(Move(Move::PIECES::UP, Move::TYPE::PRIME));

	// return the piece location
	return piece;
}

void insertCorner(Cube& cube, LOCATION piece, std::vector<Move>& solution)
{
	// if cross color is facing up
	if (piece.face == FACE::UP)
	{
		std::pair<LOCATION, LOCATION> adjs = cube.getAdjacentCorner(piece);
		// work with the adjacent piece in position 0
		LOCATION adj = adjs.first.idx == 0 ? adjs.first : adjs.second;
		// turn adjacent face clockwise
		solution.push_back(cube.move(adj.face));
		// turn up face twice
		solution.push_back(cube.parseMove("U2"));
		// turn adjacent face counter clockwise
		solution.push_back(cube.move(adj.face, Move::TYPE::PRIME));
		// turn up face counter clockwise
		solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
		// now piece is in an easier case, so recurse
		insertCorner(cube, { adj.face, 0 }, solution);
	}
	// if cross color is in position 0
	else if (piece.idx == 0)
	{
		// turn face clockwise
		solution.push_back(cube.move(piece.face));
		// turn up face clockwise
		solution.push_back(cube.move(FACE::UP));
		// turn face counter clockwise
		solution.push_back(cube.move(piece.face, Move::TYPE::PRIME));
	}
	// if cross color is in position 2
	else if (piece.idx == 2)
	{
		// turn face counter clockwise
		solution.push_back(cube.move(piece.face, Move::TYPE::PRIME));
		// turn up face counter clockwise
		solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
		// turn face clockwise
		solution.push_back(cube.move(piece.face));
	}
}

void solveCorner(Cube& cube, LOCATION piece, std::vector<Move>& solution)
{
	if (cube.isPieceSolved(piece))
		return;

	piece = bringCornerToTop(cube, piece, solution);
	piece = moveCornerOverCenter(cube, piece, solution);
	insertCorner(cube, piece, solution);
}

/**
* Solve the corners on the given cube.
*
* Assumes that the cross is solved and oriented down.
*/
void solveCorners(Cube& cube, std::vector<Move>& solution)
{
	// get the cross color
	COLOR color = cube.getCenter(FACE::DOWN);

	// solve corners
	std::pair<LOCATION, bool> cornerLoc = findUnsolvedCorner(cube, color);
	while (cornerLoc.second)
	{
		solveCorner(cube, cornerLoc.first, solution);
		solution.push_back(Move(Move::PIECES::Y, Move::TYPE::NO_MOVE));
		cornerLoc = findUnsolvedCorner(cube, color);
	}
}