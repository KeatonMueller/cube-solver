#include <iostream>

#include "Cube.h"
#include "Util.h"

Cube::Cube() { reset(); }

/**
 * Reset the cube to the solved state.
 */
void Cube::reset()
{
	// fill in the sticker colors for the cube
	uint64_t centerStickers = 0;
	// i is iterating over the FACE and COLOR enum simultaneously
	for (uint64_t i = 0; i < 6; i++)
	{
		// separated out for clarity
		COLOR color = (COLOR)(i + 1);
		FACE face = (FACE)i;
		// int containing all 8 sticker colors
		uint64_t faceStickers = 0;
		for (uint8_t sticker = 0; sticker < 8; sticker++)
		{
			faceStickers |= (uint64_t)color << (sticker * 8);
		}
		stickers[(uint64_t)face] = faceStickers;
		centerStickers |= (uint64_t)color << ((7 - i) * 8);
	}
	stickers[6] = centerStickers;
}

/**
 * Check if the cube state is solved.
 */
bool Cube::isSolved()
{
	// for every face
	for (uint8_t faceIdx = 0; faceIdx < 6; faceIdx++)
	{
		// find the center color
		COLOR centerColor = getCenter((FACE)faceIdx);
		// ensure all other stickers match the center color
		for (uint8_t stickerIdx = 0; stickerIdx < 8; stickerIdx++)
		{
			if (getSticker({ (FACE)faceIdx, stickerIdx }) != centerColor)
				return false;
		}
	}
	return true;
}

/**
 * Copy the state stored in the given string.
 * 
 * The string is 54 characters, each character corresponding to the
 * color of a particular sticker on the cube.
 * 
 * Consecutive 9 characters correspond to a single face, and the
 * faces are stored in the following order: Up, Down, Front, Back, Right, Left.
 * 
 * For any given face, the 9 stickers are stored reading left-to-right, up-and-down.
 * That means the following sequence: RGYBORGWY corresponds to this:
 * 
 *  RGY
 *  BOR
 *  GWY
 */
void Cube::copyState(std::string& state)
{
	FACE face;
	uint8_t idx;
	for (uint8_t i = 0; i < 54; i++)
	{
		face = (FACE)(i / 9);
		idx = i % 9;
		if (idx < 3 || idx == 6)
			setSticker({ face, idx }, getCharColor(state[i]));
		else if (idx == 3)
			setSticker({ face, 7 }, getCharColor(state[i]));
		else if (idx == 4)
			setCenter(face, getCharColor(state[i]));
		else if (idx == 5)
			setSticker({ face, 3 }, getCharColor(state[i]));
		else if (idx == 7)
			setSticker({ face, 5 }, getCharColor(state[i]));
		else if (idx == 8)
			setSticker({ face, 4 }, getCharColor(state[i]));
	}
}

/**
 * Return the FACE value opposite the given face.
 */
Cube::FACE Cube::getOppositeFace(FACE face)
{
	switch (face)
	{
	case FACE::UP:
		return FACE::DOWN;
	case FACE::DOWN:
		return FACE::UP;
	case FACE::FRONT:
		return FACE::BACK;
	case FACE::BACK:
		return FACE::FRONT;
	case FACE::RIGHT:
		return FACE::LEFT;
	case FACE::LEFT:
		return FACE::RIGHT;
	}
	return (FACE)0;
}

/**
 * Return the FACE value adjacent to the given face in the given
 * direction.
 */
Cube::FACE Cube::getAdjacentFace(FACE face, const std::string& dir)
{
	switch (face)
	{
	case FACE::UP:
		if (dir == "x")
			return FACE::BACK;
		else if (dir == "xPrime")
			return FACE::FRONT;
		else if (dir == "z")
			return FACE::RIGHT;
		else if (dir == "zPrime")
			return FACE::LEFT;
	case FACE::DOWN:
		if (dir == "x")
			return FACE::FRONT;
		else if (dir == "xPrime")
			return FACE::BACK;
		else if (dir == "z")
			return FACE::LEFT;
		else if (dir == "zPrime")
			return FACE::RIGHT;
	case FACE::FRONT:
		if (dir == "x")
			return FACE::UP;
		else if (dir == "xPrime")
			return FACE::DOWN;
		else if (dir == "y")
			return FACE::LEFT;
		else if (dir == "yPrime")
			return FACE::RIGHT;
	case FACE::BACK:
		if (dir == "x")
			return FACE::DOWN;
		else if (dir == "xPrime")
			return FACE::UP;
		else if (dir == "y")
			return FACE::RIGHT;
		else if (dir == "yPrime")
			return FACE::LEFT;
	case FACE::RIGHT:
		if (dir == "y")
			return FACE::FRONT;
		else if (dir == "yPrime")
			return FACE::BACK;
		else if (dir == "z")
			return FACE::DOWN;
		else if (dir == "zPrime")
			return FACE::UP;
	case FACE::LEFT:
		if (dir == "y")
			return FACE::BACK;
		else if (dir == "yPrime")
			return FACE::FRONT;
		else if (dir == "z")
			return FACE::UP;
		else if (dir == "zPrime")
			return FACE::DOWN;
	}
	return (FACE)0;
}

/**
 * Return the FACE value adjacent to the given face in the given
 * direction, with the given number of rotations.
 */
Cube::FACE Cube::getRelativeFace(FACE face, const std::string& dir, uint8_t numRotations)
{
	for (uint8_t i = 0; i < numRotations; i++)
		face = getAdjacentFace(face, dir);
	return face;
}

/**
 * Return a 64-bit integer containing the colors
 * of the stickers for the requested face.
 */
uint64_t Cube::getFace(FACE f) { return stickers[(uint8_t)f]; }

/**
 * Return the center sticker's COLOR value of the requested face.
 */
Cube::COLOR Cube::getCenter(FACE f) { return (COLOR)(stickers[6] >> ((7 - (uint8_t)f) * 8)); }

/**
 * Return the COLOR value of the requested sticker on the given face.
 */
Cube::COLOR Cube::getSticker(LOCATION l) { return (COLOR)(stickers[(uint8_t)l.face] >> ((7 - l.idx) * 8)); }

/**
 * Return the location of the sticker adjacent to the given location.
 *
 * Given location value is assumed to be an edge
 */
Cube::LOCATION Cube::getAdjacentEdge(LOCATION loc)
{
	// fail if provided location isn't an edge
	if (loc.idx % 2 == 0)
		return { (FACE)6, 8 };

	switch (loc.face)
	{
	case (FACE::UP):
		switch (loc.idx)
		{
		case 1:
			return { FACE::BACK, 1 };
		case 3:
			return { FACE::RIGHT, 1 };
		case 5:
			return { FACE::FRONT, 1 };
		case 7:
			return { FACE::LEFT, 1 };
		}
	case (FACE::DOWN):
		switch (loc.idx)
		{
		case 1:
			return { FACE::FRONT, 5 };
		case 3:
			return { FACE::RIGHT, 5 };
		case 5:
			return { FACE::BACK, 5 };
		case 7:
			return { FACE::LEFT, 5 };
		}
	case (FACE::FRONT):
		switch (loc.idx)
		{
		case 1:
			return { FACE::UP, 5 };
		case 3:
			return { FACE::RIGHT, 7 };
		case 5:
			return { FACE::DOWN, 1 };
		case 7:
			return { FACE::LEFT, 3 };
		}
	case (FACE::BACK):
		switch (loc.idx)
		{
		case 1:
			return { FACE::UP, 1 };
		case 3:
			return { FACE::LEFT, 7 };
		case 5:
			return { FACE::DOWN, 5 };
		case 7:
			return { FACE::RIGHT, 3 };
		}
	case (FACE::RIGHT):
		switch (loc.idx)
		{
		case 1:
			return { FACE::UP, 3 };
		case 3:
			return { FACE::BACK, 7 };
		case 5:
			return { FACE::DOWN, 3 };
		case 7:
			return { FACE::FRONT, 3 };
		}
	case (FACE::LEFT):
		switch (loc.idx)
		{
		case 1:
			return { FACE::UP, 7 };
		case 3:
			return { FACE::FRONT, 7 };
		case 5:
			return { FACE::DOWN, 7 };
		case 7:
			return { FACE::BACK, 3 };
		}
	default:
		// non-edge supplied; behavior undefined
		return { (FACE)0, 0 };
	}
}

/**
 * Return the locations of the stickers adjacent to the given location.
 *
 * It is the responsibility of the caller to ensure that only a corner
 * piece location is supplied to this function.
 *
 * The order of the returned adjacent pieces always follows the following
 * order: UP, DOWN, FRONT, BACK, RIGHT, LEFT.
 */
std::pair<Cube::LOCATION, Cube::LOCATION> Cube::getAdjacentCorner(LOCATION loc)
{
	switch (loc.face)
	{
	case (FACE::UP):
		switch (loc.idx)
		{
		case 0:
			return std::make_pair(LOCATION({ FACE::BACK, 2 }), LOCATION({ FACE::LEFT, 0 }));
		case 2:
			return std::make_pair(LOCATION({ FACE::BACK, 0 }), LOCATION({ FACE::RIGHT, 2 }));
		case 4:
			return std::make_pair(LOCATION({ FACE::FRONT, 2 }), LOCATION({ FACE::RIGHT, 0 }));
		case 6:
			return std::make_pair(LOCATION({ FACE::FRONT, 0 }), LOCATION({ FACE::LEFT, 2 }));
		}
	case (FACE::DOWN):
		switch (loc.idx)
		{
		case 0:
			return std::make_pair(LOCATION({ FACE::FRONT, 6 }), LOCATION({ FACE::LEFT, 4 }));
		case 2:
			return std::make_pair(LOCATION({ FACE::FRONT, 4 }), LOCATION({ FACE::RIGHT, 6 }));
		case 4:
			return std::make_pair(LOCATION({ FACE::BACK, 6 }), LOCATION({ FACE::RIGHT, 4 }));
		case 6:
			return std::make_pair(LOCATION({ FACE::BACK, 4 }), LOCATION({ FACE::LEFT, 6 }));
		}
	case (FACE::FRONT):
		switch (loc.idx)
		{
		case 0:
			return std::make_pair(LOCATION({ FACE::UP, 6 }), LOCATION({ FACE::LEFT, 2 }));
		case 2:
			return std::make_pair(LOCATION({ FACE::UP, 4 }), LOCATION({ FACE::RIGHT, 0 }));
		case 4:
			return std::make_pair(LOCATION({ FACE::DOWN, 2 }), LOCATION({ FACE::RIGHT, 6 }));
		case 6:
			return std::make_pair(LOCATION({ FACE::DOWN, 0 }), LOCATION({ FACE::LEFT, 4 }));
		}
	case (FACE::BACK):
		switch (loc.idx)
		{
		case 0:
			return std::make_pair(LOCATION({ FACE::UP, 2 }), LOCATION({ FACE::RIGHT, 2 }));
		case 2:
			return std::make_pair(LOCATION({ FACE::UP, 0 }), LOCATION({ FACE::LEFT, 0 }));
		case 4:
			return std::make_pair(LOCATION({ FACE::DOWN, 6 }), LOCATION({ FACE::LEFT, 6 }));
		case 6:
			return std::make_pair(LOCATION({ FACE::DOWN, 4 }), LOCATION({ FACE::RIGHT, 4 }));
		}
	case (FACE::RIGHT):
		switch (loc.idx)
		{
		case 0:
			return std::make_pair(LOCATION({ FACE::UP, 4 }), LOCATION({ FACE::FRONT, 2 }));
		case 2:
			return std::make_pair(LOCATION({ FACE::UP, 2 }), LOCATION({ FACE::BACK, 0 }));
		case 4:
			return std::make_pair(LOCATION({ FACE::DOWN, 4 }), LOCATION({ FACE::BACK, 6 }));
		case 6:
			return std::make_pair(LOCATION({ FACE::DOWN, 2 }), LOCATION({ FACE::FRONT, 4 }));
		}
	case (FACE::LEFT):
		switch (loc.idx)
		{
		case 0:
			return std::make_pair(LOCATION({ FACE::UP, 0 }), LOCATION({ FACE::BACK, 2 }));
		case 2:
			return std::make_pair(LOCATION({ FACE::UP, 6 }), LOCATION({ FACE::FRONT, 0 }));
		case 4:
			return std::make_pair(LOCATION({ FACE::DOWN, 0 }), LOCATION({ FACE::FRONT, 6 }));
		case 6:
			return std::make_pair(LOCATION({ FACE::DOWN, 6 }), LOCATION({ FACE::BACK, 4 }));
		}
	}
	return std::make_pair(LOCATION({ (FACE)0, 0 }), LOCATION({ (FACE)0, 0 }));
}

/**
 * Perform a move of the given type on the requested face.
 *
 * Return a Move object corresponding to the move performed.
 */
Move Cube::move(FACE face, Move::TYPE type)
{
	// get the string of the requested face
	std::string move = "";
	switch (face)
	{
	case FACE::UP:
		move = "U";
		break;
	case FACE::DOWN:
		move = "D";
		break;
	case FACE::FRONT:
		move = "F";
		break;
	case FACE::BACK:
		move = "B";
		break;
	case FACE::RIGHT:
		move = "R";
		break;
	case FACE::LEFT:
		move = "L";
		break;
	}

	// parse move based on type
	if (type == Move::TYPE::NORMAL)
		return parseMove(move);
	else if (type == Move::TYPE::PRIME)
		return parseMove(move + "\'");
	else if (type == Move::TYPE::DOUBLE)
		return parseMove(move + "2");

	// invalid move
	return Move((Move::PIECES)0, Move::TYPE::NO_MOVE);
}

/**
 * Determine if the piece at the given location is solved.
 */
bool Cube::isPieceSolved(LOCATION loc)
{
	if (loc.idx % 2 == 0)
		return isCornerSolved(loc);
	return isEdgeSolved(loc);
}

/**
 * Determine if the edge piece found at the given location is solved.
 */
bool Cube::isEdgeSolved(LOCATION loc)
{
	// given sticker must match center piece
	if (getCenter(loc.face) != getSticker(loc))
		return false;

	// get the adjacent edge sticker
	LOCATION adj = getAdjacentEdge(loc);

	// solved if adjacent sticker matches its center as well
	return getCenter(adj.face) == getSticker(adj);
}

/**
 * Determine if the corner piece found at the given location is solved.
 */
bool Cube::isCornerSolved(LOCATION loc)
{
	// given sticker must match center piece
	if (getCenter(loc.face) != getSticker(loc))
		return false;

	// so must the two adjacent stickers
	std::pair<LOCATION, LOCATION> adj = getAdjacentCorner(loc);
	if (getCenter(adj.first.face) != getSticker(adj.first))
		return false;
	return getCenter(adj.second.face) == getSticker(adj.second);
}

/**
 * Set the given face to the given value.
 */
void Cube::setFace(FACE f, uint64_t value) { stickers[(uint8_t)f] = value; }

/**
 * Set the given face's center to the given color.
 */
void Cube::setCenter(FACE f, COLOR c)
{
	uint8_t numBits = (7 - (uint64_t)f) * 8;
	uint64_t centerMask = (uint64_t)0xff << numBits;
	stickers[6] = (stickers[6] & ~centerMask) | ((uint64_t)c << numBits);
}

/**
 * Set the given location to the given color.
 */
void Cube::setSticker(LOCATION l, COLOR c)
{
	uint64_t face = getFace(l.face);
	face &= ~((uint64_t)0xff << ((7 - l.idx) * 8));
	face |= ((uint64_t)c << ((7 - l.idx) * 8));
	setFace(l.face, face);
}

/**
 * Perform every move present in the string of moves.
 *
 * Return a vector of Move objects for each extracted move.
 */
std::vector<Move> Cube::readMoves(const std::string& moves)
{
	std::vector<Move> moveVector;
	Move move(Move::PIECES::UP, Move::TYPE::NO_MOVE);
	uint8_t moveIdx = 0;
	while (moveIdx < moves.length())
	{
		if (moveIdx < moves.length() - 1 && (moves[moveIdx + 1] == '\'' || moves[moveIdx + 1] == '2'))
		{
			move = parseMove(moves.substr(moveIdx, 2));
			moveIdx += 2;

			if (move.type != Move::TYPE::NO_MOVE)
				moveVector.push_back(move);
		}
		else
		{
			move = parseMove(moves.substr(moveIdx, 1));
			moveIdx += 1;

			if (move.type != Move::TYPE::NO_MOVE)
				moveVector.push_back(move);
		}
	}
	return moveVector;
}

/**
 * Execute the moves in the given vector.
 */
void Cube::executeMoves(std::vector<Move>& moves)
{
	for (Move& move : moves)
		parseMove(move.toString());
}

/**
 * Perform the single move represented by the given string.
 *
 * The move may be clockwise 90 degrees, counter clockwise 90 degrees,
 * or a 180 degree turn.
 *
 * Return the corresponding Move object.
 */
Move Cube::parseMove(const std::string& move)
{
	if (move == "U")
	{
		u();
		return Move(Move::PIECES::UP, Move::TYPE::NORMAL);
	}
	else if (move == "U\'")
	{
		uPrime();
		return Move(Move::PIECES::UP, Move::TYPE::PRIME);
	}
	else if (move == "U2")
	{
		u();
		u();
		return Move(Move::PIECES::UP, Move::TYPE::DOUBLE);
	}
	else if (move == "u")
	{
		uWide();
		return Move(Move::PIECES::UP_WIDE, Move::TYPE::NORMAL);
	}
	else if (move == "u\'")
	{
		uPrimeWide();
		return Move(Move::PIECES::UP_WIDE, Move::TYPE::PRIME);
	}
	else if (move == "u2")
	{
		uWide();
		uWide();
		return Move(Move::PIECES::UP_WIDE, Move::TYPE::DOUBLE);
	}
	else if (move == "D")
	{
		d();
		return Move(Move::PIECES::DOWN, Move::TYPE::NORMAL);
	}
	else if (move == "D\'")
	{
		dPrime();
		return Move(Move::PIECES::DOWN, Move::TYPE::PRIME);
	}
	else if (move == "D2")
	{
		d();
		d();
		return Move(Move::PIECES::DOWN, Move::TYPE::DOUBLE);
	}
	else if (move == "d")
	{
		dWide();
		return Move(Move::PIECES::DOWN_WIDE, Move::TYPE::NORMAL);
	}
	else if (move == "d\'")
	{
		dPrimeWide();
		return Move(Move::PIECES::DOWN_WIDE, Move::TYPE::PRIME);
	}
	else if (move == "d2")
	{
		dWide();
		dWide();
		return Move(Move::PIECES::DOWN_WIDE, Move::TYPE::DOUBLE);
	}
	else if (move == "F")
	{
		f();
		return Move(Move::PIECES::FRONT, Move::TYPE::NORMAL);
	}
	else if (move == "F\'")
	{
		fPrime();
		return Move(Move::PIECES::FRONT, Move::TYPE::PRIME);
	}
	else if (move == "F2")
	{
		f();
		f();
		return Move(Move::PIECES::FRONT, Move::TYPE::DOUBLE);
	}
	else if (move == "f")
	{
		fWide();
		return Move(Move::PIECES::FRONT_WIDE, Move::TYPE::NORMAL);
	}
	else if (move == "f\'")
	{
		fPrimeWide();
		return Move(Move::PIECES::FRONT_WIDE, Move::TYPE::PRIME);
	}
	else if (move == "f2")
	{
		fWide();
		fWide();
		return Move(Move::PIECES::FRONT_WIDE, Move::TYPE::DOUBLE);
	}
	else if (move == "B")
	{
		b();
		return Move(Move::PIECES::BACK, Move::TYPE::NORMAL);
	}
	else if (move == "B\'")
	{
		bPrime();
		return Move(Move::PIECES::BACK, Move::TYPE::PRIME);
	}
	else if (move == "B2")
	{
		b();
		b();
		return Move(Move::PIECES::BACK, Move::TYPE::DOUBLE);
	}
	else if (move == "b")
	{
		bWide();
		return Move(Move::PIECES::BACK_WIDE, Move::TYPE::NORMAL);
	}
	else if (move == "b\'")
	{
		bPrimeWide();
		return Move(Move::PIECES::BACK_WIDE, Move::TYPE::PRIME);
	}
	else if (move == "b2")
	{
		bWide();
		bWide();
		return Move(Move::PIECES::BACK_WIDE, Move::TYPE::DOUBLE);
	}
	else if (move == "R")
	{
		r();
		return Move(Move::PIECES::RIGHT, Move::TYPE::NORMAL);
	}
	else if (move == "R\'")
	{
		rPrime();
		return Move(Move::PIECES::RIGHT, Move::TYPE::PRIME);
	}
	else if (move == "R2")
	{
		r();
		r();
		return Move(Move::PIECES::RIGHT, Move::TYPE::DOUBLE);
	}
	else if (move == "r")
	{
		rWide();
		return Move(Move::PIECES::RIGHT_WIDE, Move::TYPE::NORMAL);
	}
	else if (move == "r\'")
	{
		rPrimeWide();
		return Move(Move::PIECES::RIGHT_WIDE, Move::TYPE::PRIME);
	}
	else if (move == "r2")
	{
		rWide();
		rWide();
		return Move(Move::PIECES::RIGHT_WIDE, Move::TYPE::DOUBLE);
	}
	else if (move == "L")
	{
		l();
		return Move(Move::PIECES::LEFT, Move::TYPE::NORMAL);
	}
	else if (move == "L\'")
	{
		lPrime();
		return Move(Move::PIECES::LEFT, Move::TYPE::PRIME);
	}
	else if (move == "L2")
	{
		l();
		l();
		return Move(Move::PIECES::LEFT, Move::TYPE::DOUBLE);
	}
	else if (move == "l")
	{
		lWide();
		return Move(Move::PIECES::LEFT_WIDE, Move::TYPE::NORMAL);
	}
	else if (move == "l\'")
	{
		lPrimeWide();
		return Move(Move::PIECES::LEFT_WIDE, Move::TYPE::PRIME);
	}
	else if (move == "l2")
	{
		lWide();
		lWide();
		return Move(Move::PIECES::LEFT_WIDE, Move::TYPE::DOUBLE);
	}
	else if (move == "M")
	{
		m();
		return Move(Move::PIECES::M_SLICE, Move::TYPE::NORMAL);
	}
	else if (move == "M\'")
	{
		mPrime();
		return Move(Move::PIECES::M_SLICE, Move::TYPE::PRIME);
	}
	else if (move == "M2")
	{
		m();
		m();
		return Move(Move::PIECES::M_SLICE, Move::TYPE::DOUBLE);
	}
	else if (move == "E")
	{
		e();
		return Move(Move::PIECES::E_SLICE, Move::TYPE::NORMAL);
	}
	else if (move == "E\'")
	{
		ePrime();
		return Move(Move::PIECES::E_SLICE, Move::TYPE::PRIME);
	}
	else if (move == "E2")
	{
		e();
		e();
		return Move(Move::PIECES::E_SLICE, Move::TYPE::DOUBLE);
	}
	else if (move == "S")
	{
		s();
		return Move(Move::PIECES::S_SLICE, Move::TYPE::NORMAL);
	}
	else if (move == "S\'")
	{
		sPrime();
		return Move(Move::PIECES::S_SLICE, Move::TYPE::PRIME);
	}
	else if (move == "S2")
	{
		s();
		s();
		return Move(Move::PIECES::S_SLICE, Move::TYPE::DOUBLE);
	}
	else if (move == "X" || move == "x")
	{
		x();
		return Move(Move::PIECES::X, Move::TYPE::NORMAL);
	}
	else if (move == "X\'" || move == "x\'")
	{
		xPrime();
		return Move(Move::PIECES::X, Move::TYPE::PRIME);
	}
	else if (move == "X2" || move == "x2")
	{
		x();
		x();
		return Move(Move::PIECES::X, Move::TYPE::DOUBLE);
	}
	else if (move == "Y" || move == "y")
	{
		y();
		return Move(Move::PIECES::Y, Move::TYPE::NORMAL);
	}
	else if (move == "Y\'" || move == "y\'")
	{
		yPrime();
		return Move(Move::PIECES::Y, Move::TYPE::PRIME);
	}
	else if (move == "Y2" || move == "y2")
	{
		y();
		y();
		return Move(Move::PIECES::Y, Move::TYPE::DOUBLE);
	}
	else if (move == "Z" || move == "z")
	{
		z();
		return Move(Move::PIECES::Z, Move::TYPE::NORMAL);
	}
	else if (move == "Z\'" || move == "z\'")
	{
		zPrime();
		return Move(Move::PIECES::Z, Move::TYPE::PRIME);
	}
	else if (move == "Z2" || move == "z2")
	{
		z();
		z();
		return Move(Move::PIECES::Z, Move::TYPE::DOUBLE);
	}
	// unrecognized move type
	return Move(Move::PIECES::UP, Move::TYPE::NO_MOVE);
}

/**
 * Perform a clockwise rotation of the up face.
 */
void Cube::u()
{
	// turn the up face
	setFace(FACE::UP, rotateRight(getFace(FACE::UP), 16));

	// turn the adjacent stickers on the front, left, back, and right faces
	uint64_t toSave = getFace(FACE::FRONT) & upMask;
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~upMask) | (getFace(FACE::RIGHT) & upMask));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~upMask) | (getFace(FACE::BACK) & upMask));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~upMask) | (getFace(FACE::LEFT) & upMask));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~upMask) | toSave);
}

/**
 * Perform a counter clockwise rotation of the up face.
 */
void Cube::uPrime()
{
	// turn the up face
	setFace(FACE::UP, rotateLeft(getFace(FACE::UP), 16));

	// turn the adjacent stickers on the front, left, back, and right faces
	uint64_t toSave = getFace(FACE::FRONT) & upMask;
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~upMask) | (getFace(FACE::LEFT) & upMask));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~upMask) | (getFace(FACE::BACK) & upMask));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~upMask) | (getFace(FACE::RIGHT) & upMask));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~upMask) | toSave);
}

/**
 * Perform a clockwise wide U.
 */
void Cube::uWide()
{
	u();
	ePrime();
}

/**
 * Perform a counter clockwise wide U.
 */
void Cube::uPrimeWide()
{
	uPrime();
	e();
}

/**
 * Perform a clockwise rotation of the down face.
 */
void Cube::d()
{
	// turn the down face
	setFace(FACE::DOWN, rotateRight(getFace(FACE::DOWN), 16));

	// turn the adjacent stickers on the front, right, back, and left faces
	uint64_t toSave = getFace(FACE::FRONT) & downMask;
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~downMask) | (getFace(FACE::LEFT) & downMask));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~downMask) | (getFace(FACE::BACK) & downMask));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~downMask) | (getFace(FACE::RIGHT) & downMask));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~downMask) | toSave);
}

/**
 * Perform a counter clockwise rotation of the down face.
 */
void Cube::dPrime()
{
	// turn the down face
	setFace(FACE::DOWN, rotateLeft(getFace(FACE::DOWN), 16));

	// turn the adjacent stickers on the front, right, back, and left faces
	uint64_t toSave = getFace(FACE::FRONT) & downMask;
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~downMask) | (getFace(FACE::RIGHT) & downMask));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~downMask) | (getFace(FACE::BACK) & downMask));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~downMask) | (getFace(FACE::LEFT) & downMask));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~downMask) | toSave);
}

/**
 * Perform a clockwise wide D.
 */
void Cube::dWide()
{
	d();
	e();
}

/**
 * Perform a counter clockwise wide D.
 */
void Cube::dPrimeWide()
{
	dPrime();
	ePrime();
}

/**
 * Perform a clockwise rotation of the front face.
 */
void Cube::f()
{
	// turn the front face
	setFace(FACE::FRONT, rotateRight(getFace(FACE::FRONT), 16));

	// turn the adjacent stickers on the up, right, bottom, and left faces
	uint64_t toSave = getFace(FACE::UP) & downMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~downMask) | ((getFace(FACE::LEFT) & rightMask) >> 16));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~rightMask) | ((getFace(FACE::DOWN) & upMask) >> 16));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~upMask) | rotateRight(getFace(FACE::RIGHT) & leftMask, 16));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~leftMask) | rotateRight(toSave, 16));
}

/**
 * Perform a counter clockwise rotation of the front face.
 */
void Cube::fPrime()
{
	// turn the front face
	setFace(FACE::FRONT, rotateLeft(getFace(FACE::FRONT), 16));

	// turn the adjacent stickers on the up, right, bottom, and left faces
	uint64_t toSave = getFace(FACE::UP) & downMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~downMask) | rotateLeft(getFace(FACE::RIGHT) & leftMask, 16));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~leftMask) | rotateLeft(getFace(FACE::DOWN) & upMask, 16));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~upMask) | rotateLeft(getFace(FACE::LEFT) & rightMask, 16));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~rightMask) | (toSave << 16));
}

/**
 * Perform a clockwise wide F.
 */
void Cube::fWide()
{
	f();
	s();
}

/**
 * Perform a counter clockwise wide F.
 */
void Cube::fPrimeWide()
{
	fPrime();
	sPrime();
}

/**
 * Perform a clockwise rotation of the back face.
 */
void Cube::b()
{
	// turn the back face
	setFace(FACE::BACK, rotateRight(getFace(FACE::BACK), 16));

	// turn the adjacent stickers on the up, left, down, and right faces
	uint64_t toSave = getFace(FACE::UP) & upMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~upMask) | ((getFace(FACE::RIGHT) & rightMask) << 16));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~rightMask) | ((getFace(FACE::DOWN) & downMask) << 16));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~downMask) | rotateLeft(getFace(FACE::LEFT) & leftMask, 16));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~leftMask) | rotateLeft(toSave, 16));
}

/**
 * Perform a counter clockwise rotation of the back face.
 */
void Cube::bPrime()
{
	// turn the back face
	setFace(FACE::BACK, rotateLeft(getFace(FACE::BACK), 16));

	// turn the adjacent stickers on the up, left, down, and right faces
	uint64_t toSave = getFace(FACE::UP) & upMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~upMask) | rotateRight(getFace(FACE::LEFT) & leftMask, 16));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~leftMask) | rotateRight(getFace(FACE::DOWN) & downMask, 16));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~downMask) | ((getFace(FACE::RIGHT) & rightMask) >> 16));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~rightMask) | (toSave >> 16));
}

/**
 * Perform a clockwise wide B.
 */
void Cube::bWide()
{
	b();
	sPrime();
}

/**
 * Perform a counter clockwise wide B.
 */
void Cube::bPrimeWide()
{
	bPrime();
	s();
}

/**
 * Perform a clockwise rotation of the right face.
 */
void Cube::r()
{
	// turn the right face
	setFace(FACE::RIGHT, rotateRight(getFace(FACE::RIGHT), 16));

	// turn the adjacent stickers on the up, back, down, and front faces
	uint64_t toSave = getFace(FACE::UP) & rightMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~rightMask) | (getFace(FACE::FRONT) & rightMask));
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~rightMask) | (getFace(FACE::DOWN) & rightMask));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~rightMask) | rotateRight(getFace(FACE::BACK) & leftMask, 32));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~leftMask) | rotateLeft(toSave, 32));
}

/**
 * Perform a counter clockwise rotation of the right face.
 */
void Cube::rPrime()
{
	// turn the right face
	setFace(FACE::RIGHT, rotateLeft(getFace(FACE::RIGHT), 16));

	// turn the adjacent stickers on the up, back, down, and front faces
	uint64_t toSave = getFace(FACE::UP) & rightMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~rightMask) | rotateRight(getFace(FACE::BACK) & leftMask, 32));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~leftMask) | rotateRight(getFace(FACE::DOWN) & rightMask, 32));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~rightMask) | (getFace(FACE::FRONT) & rightMask));
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~rightMask) | toSave);
}

/**
 * Perform a clockwise wide R.
 */
void Cube::rWide()
{
	r();
	mPrime();
}

/**
 * Perform a counter clockwise wide R.
 */
void Cube::rPrimeWide()
{
	rPrime();
	m();
}

/**
 * Perform a clockwise rotation of the left face.
 */
void Cube::l()
{
	// turn the left face
	setFace(FACE::LEFT, rotateRight(getFace(FACE::LEFT), 16));

	// turn the adjacent stickers on the up, front, down, and back faces
	uint64_t toSave = getFace(FACE::UP) & leftMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~leftMask) | rotateRight(getFace(FACE::BACK) & rightMask, 32));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~rightMask) | rotateRight(getFace(FACE::DOWN) & leftMask, 32));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~leftMask) | (getFace(FACE::FRONT) & leftMask));
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~leftMask) | toSave);
}

/**
 * Perform a counter clockwise rotation of the left face.
 */
void Cube::lPrime()
{
	// turn the left face
	setFace(FACE::LEFT, rotateLeft(getFace(FACE::LEFT), 16));

	// turn the adjacent stickers on the up, front, down, and back faces
	uint64_t toSave = getFace(FACE::UP) & leftMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~leftMask) | (getFace(FACE::FRONT) & leftMask));
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~leftMask) | (getFace(FACE::DOWN) & leftMask));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~leftMask) | rotateRight(getFace(FACE::BACK) & rightMask, 32));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~rightMask) | rotateRight(toSave, 32));
}

/**
 * Perform a clockwise wide L.
 */
void Cube::lWide()
{
	l();
	m();
}

/**
 * Perform a counter clockwise wide L.
 */
void Cube::lPrimeWide()
{
	lPrime();
	mPrime();
}

/**
 * Perform a clockwise rotation of the M slice.
 */
void Cube::m()
{
	// rotate the centers
	COLOR toSaveCenter = getCenter(FACE::UP);
	setCenter(FACE::UP, getCenter(FACE::BACK));
	setCenter(FACE::BACK, getCenter(FACE::DOWN));
	setCenter(FACE::DOWN, getCenter(FACE::FRONT));
	setCenter(FACE::FRONT, toSaveCenter);

	// rotate the edge pieces
	uint64_t toSaveEdges = getFace(FACE::UP) & middleColMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~middleColMask) | rotateRight(getFace(FACE::BACK) & middleColMask, 32));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~middleColMask) | rotateRight(getFace(FACE::DOWN) & middleColMask, 32));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~middleColMask) | (getFace(FACE::FRONT) & middleColMask));
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~middleColMask) | toSaveEdges);
}

/**
 * Perform a counter clockwise rotation of the M slice.
 */
void Cube::mPrime()
{
	// rotate the centers
	COLOR toSaveCenter = getCenter(FACE::UP);
	setCenter(FACE::UP, getCenter(FACE::FRONT));
	setCenter(FACE::FRONT, getCenter(FACE::DOWN));
	setCenter(FACE::DOWN, getCenter(FACE::BACK));
	setCenter(FACE::BACK, toSaveCenter);

	// rotate the edge pieces
	uint64_t toSaveEdges = getFace(FACE::UP) & middleColMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~middleColMask) | (getFace(FACE::FRONT) & middleColMask));
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~middleColMask) | (getFace(FACE::DOWN) & middleColMask));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~middleColMask) | rotateRight(getFace(FACE::BACK) & middleColMask, 32));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~middleColMask) | rotateRight(toSaveEdges, 32));
}

/**
 * Perform a clockwise rotation of the E slice.
 */
void Cube::e()
{
	// rotate the centers
	COLOR toSaveCenter = getCenter(FACE::FRONT);
	setCenter(FACE::FRONT, getCenter(FACE::LEFT));
	setCenter(FACE::LEFT, getCenter(FACE::BACK));
	setCenter(FACE::BACK, getCenter(FACE::RIGHT));
	setCenter(FACE::RIGHT, toSaveCenter);

	// rotate the edge pieces
	uint64_t toSaveEdges = getFace(FACE::FRONT) & middleRowMask;
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~middleRowMask) | (getFace(FACE::LEFT) & middleRowMask));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~middleRowMask) | (getFace(FACE::BACK) & middleRowMask));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~middleRowMask) | (getFace(FACE::RIGHT) & middleRowMask));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~middleRowMask) | toSaveEdges);
}

/**
 * Perform a counter clockwise rotation of the E slice.
 */
void Cube::ePrime()
{
	// rotate the centers
	COLOR toSaveCenter = getCenter(FACE::FRONT);
	setCenter(FACE::FRONT, getCenter(FACE::RIGHT));
	setCenter(FACE::RIGHT, getCenter(FACE::BACK));
	setCenter(FACE::BACK, getCenter(FACE::LEFT));
	setCenter(FACE::LEFT, toSaveCenter);

	// rotate the edge pieces
	uint64_t toSaveEdges = getFace(FACE::FRONT) & middleRowMask;
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~middleRowMask) | (getFace(FACE::RIGHT) & middleRowMask));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~middleRowMask) | (getFace(FACE::BACK) & middleRowMask));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~middleRowMask) | (getFace(FACE::LEFT) & middleRowMask));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~middleRowMask) | toSaveEdges);
}

/**
 * Perform a clockwise rotation of the S slice.
 */
void Cube::s()
{
	// rotate the centers
	COLOR toSaveCenter = getCenter(FACE::UP);
	setCenter(FACE::UP, getCenter(FACE::LEFT));
	setCenter(FACE::LEFT, getCenter(FACE::DOWN));
	setCenter(FACE::DOWN, getCenter(FACE::RIGHT));
	setCenter(FACE::RIGHT, toSaveCenter);

	// rotate the edge pieces
	uint64_t toSaveEdges = getFace(FACE::UP) & middleRowMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~middleRowMask) | ((getFace(FACE::LEFT) & middleColMask) >> 16));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~middleColMask) | rotateRight(getFace(FACE::DOWN) & middleRowMask, 16));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~middleRowMask) | ((getFace(FACE::RIGHT) & middleColMask) >> 16));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~middleColMask) | rotateRight(toSaveEdges, 16));
}

/**
 * Perform a counter clockwise rotation of the S slice.
 */
void Cube::sPrime()
{
	// rotate the centers
	COLOR toSaveCenter = getCenter(FACE::UP);
	setCenter(FACE::UP, getCenter(FACE::RIGHT));
	setCenter(FACE::RIGHT, getCenter(FACE::DOWN));
	setCenter(FACE::DOWN, getCenter(FACE::LEFT));
	setCenter(FACE::LEFT, toSaveCenter);

	// rotate the edge pieces
	uint64_t toSaveEdges = getFace(FACE::UP) & middleRowMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~middleRowMask) | rotateLeft(getFace(FACE::RIGHT) & middleColMask, 16));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~middleColMask) | rotateLeft(getFace(FACE::DOWN) & middleRowMask, 16));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~middleRowMask) | rotateLeft(getFace(FACE::LEFT) & middleColMask, 16));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~middleColMask) | rotateLeft(toSaveEdges, 16));
}

/**
 * Perform a clockwise cube rotation on the X axis.
 * This is done in terms of outer turns and slice moves, so it's
 * less efficient.
 */
void Cube::x()
{
	r();
	lPrime();
	mPrime();
}

/**
 * Perform a counter clockwise cube rotation on the X axis.
 * This is done in terms of outer turns and slice moves, so it's
 * less efficient.
 */
void Cube::xPrime()
{
	rPrime();
	l();
	m();
}

/**
 * Perform a clockwise cube rotation on the Y axis.
 * This is done in terms of outer turns and slice moves, so it's
 * less efficient.
 */
void Cube::y()
{
	u();
	dPrime();
	ePrime();
}

/**
 * Perform a counter clockwise cube rotation on the Y axis.
 * This is done in terms of outer turns and slice moves, so it's
 * less efficient.
 */
void Cube::yPrime()
{
	uPrime();
	d();
	e();
}

/**
 * Perform a clockwise cube rotation on the Z axis.
 * This is done in terms of outer turns and slice moves, so it's
 * less efficient.
 */
void Cube::z()
{
	f();
	bPrime();
	s();
}

/**
 * Perform a counter clockwise cube rotation on the Z axis.
 * This is done in terms of outer turns and slice moves, so it's
 * less efficient.
 */
void Cube::zPrime()
{
	fPrime();
	b();
	sPrime();
}

/**
 * Get the single character value corresponding to each sticker color.
 */
char Cube::getColorChar(COLOR c)
{
	switch (c)
	{
	case COLOR::WHITE:
		return 'W';
	case COLOR::YELLOW:
		return 'Y';
	case COLOR::RED:
		return 'R';
	case COLOR::ORANGE:
		return 'O';
	case COLOR::BLUE:
		return 'B';
	case COLOR::GREEN:
		return 'G';
	default:
		return ' ';
	}
}

/**
 * Get the COLOR value corresponding to the given character.
 */
Cube::COLOR Cube::getCharColor(char c)
{
	switch (c)
	{
	case 'W':
		return COLOR::WHITE;
	case 'Y':
		return COLOR::YELLOW;
	case 'R':
		return COLOR::RED;
	case 'O':
		return COLOR::ORANGE;
	case 'B':
		return COLOR::BLUE;
	case 'G':
		return COLOR::GREEN;
	default:
		return COLOR::EMPTY;
	}
}

void Cube::printLocation(LOCATION loc)
{
	COLOR c = getSticker(loc);
	switch (loc.face)
	{
	case FACE::UP:
		std::cout << "up";
		break;
	case FACE::DOWN:
		std::cout << "down";
		break;
	case FACE::FRONT:
		std::cout << "front";
		break;
	case FACE::BACK:
		std::cout << "back";
		break;
	case FACE::RIGHT:
		std::cout << "right";
		break;
	case FACE::LEFT:
		std::cout << "left";
		break;
	}
	std::cout << " face, idx " << (int)loc.idx << " color " << getColorChar(c) << std::endl;
}

/**
 * Print the Rubik's Cube in a readable format to the console.
 *
 * While the data structure used to represent the Rubik's Cube is
 * useful for turns and efficiency, it unfortunately makes it
 * somewhat cumbersome to print.
 */
void Cube::print()
{
	// print top face indented by 4 spaces
	std::cout << "    " << getColorChar(getSticker({ FACE::UP, 0 })) << getColorChar(getSticker({ FACE::UP, 1 })) << getColorChar(getSticker({ FACE::UP, 2 })) << std::endl;
	std::cout << "    " << getColorChar(getSticker({ FACE::UP, 7 })) << getColorChar(getCenter(FACE::UP)) << getColorChar(getSticker({ FACE::UP, 3 })) << std::endl;
	std::cout << "    " << getColorChar(getSticker({ FACE::UP, 6 })) << getColorChar(getSticker({ FACE::UP, 5 })) << getColorChar(getSticker({ FACE::UP, 4 })) << std::endl;

	// print first row of left, front, right, and back faces, each separated by a
	// space
	std::cout << "\n" << getColorChar(getSticker({ FACE::LEFT, 0 })) << getColorChar(getSticker({ FACE::LEFT, 1 })) << getColorChar(getSticker({ FACE::LEFT, 2 }));
	std::cout << " " << getColorChar(getSticker({ FACE::FRONT, 0 })) << getColorChar(getSticker({ FACE::FRONT, 1 })) << getColorChar(getSticker({ FACE::FRONT, 2 }));
	std::cout << " " << getColorChar(getSticker({ FACE::RIGHT, 0 })) << getColorChar(getSticker({ FACE::RIGHT, 1 })) << getColorChar(getSticker({ FACE::RIGHT, 2 }));
	std::cout << " " << getColorChar(getSticker({ FACE::BACK, 0 })) << getColorChar(getSticker({ FACE::BACK, 1 })) << getColorChar(getSticker({ FACE::BACK, 2 })) << std::endl;

	// print second row of left, center, right, and back faces, each separated by
	// a space
	std::cout << getColorChar(getSticker({ FACE::LEFT, 7 })) << getColorChar(getCenter(FACE::LEFT)) << getColorChar(getSticker({ FACE::LEFT, 3 }));
	std::cout << " " << getColorChar(getSticker({ FACE::FRONT, 7 })) << getColorChar(getCenter(FACE::FRONT)) << getColorChar(getSticker({ FACE::FRONT, 3 }));
	std::cout << " " << getColorChar(getSticker({ FACE::RIGHT, 7 })) << getColorChar(getCenter(FACE::RIGHT)) << getColorChar(getSticker({ FACE::RIGHT, 3 }));
	std::cout << " " << getColorChar(getSticker({ FACE::BACK, 7 })) << getColorChar(getCenter(FACE::BACK)) << getColorChar(getSticker({ FACE::BACK, 3 })) << std::endl;

	// print third row of left, center, right, and back faces, each separated by a
	// space
	std::cout << getColorChar(getSticker({ FACE::LEFT, 6 })) << getColorChar(getSticker({ FACE::LEFT, 5 })) << getColorChar(getSticker({ FACE::LEFT, 4 }));
	std::cout << " " << getColorChar(getSticker({ FACE::FRONT, 6 })) << getColorChar(getSticker({ FACE::FRONT, 5 })) << getColorChar(getSticker({ FACE::FRONT, 4 }));
	std::cout << " " << getColorChar(getSticker({ FACE::RIGHT, 6 })) << getColorChar(getSticker({ FACE::RIGHT, 5 })) << getColorChar(getSticker({ FACE::RIGHT, 4 }));
	std::cout << " " << getColorChar(getSticker({ FACE::BACK, 6 })) << getColorChar(getSticker({ FACE::BACK, 5 })) << getColorChar(getSticker({ FACE::BACK, 4 })) << "\n\n";

	// print the down face indented by 4 spaces
	std::cout << "    " << getColorChar(getSticker({ FACE::DOWN, 0 })) << getColorChar(getSticker({ FACE::DOWN, 1 })) << getColorChar(getSticker({ FACE::DOWN, 2 })) << std::endl;
	std::cout << "    " << getColorChar(getSticker({ FACE::DOWN, 7 })) << getColorChar(getCenter(FACE::DOWN)) << getColorChar(getSticker({ FACE::DOWN, 3 })) << std::endl;
	std::cout << "    " << getColorChar(getSticker({ FACE::DOWN, 6 })) << getColorChar(getSticker({ FACE::DOWN, 5 })) << getColorChar(getSticker({ FACE::DOWN, 4 })) << "\n\n";
}
