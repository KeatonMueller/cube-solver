#include <iostream>

#include "Cube.h"
#include "Util.h"

Cube::Cube()
{
	// fill in the sticker colors for the cube
	uint64_t centerStickers = 0;
	// i is iterating over the FACE and COLOR enum simultaneously
	for (uint64_t i = 0; i < 6; i++)
	{
		// separated out for clarity
		COLOR color = (COLOR)i;
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
* Return a 64-bit integer containing the colors
* of the stickers for the requested face.
*/
uint64_t Cube::getFace(FACE f)
{
	return stickers[(uint8_t)f];
}

/**
* Return the center sticker's COLOR value of the requested face.
*/
Cube::COLOR Cube::getCenter(FACE f)
{
	return (COLOR)(stickers[6] >> ((7 - (uint8_t)f) * 8));
}

/**
* Return the COLOR value of the requested sticker on the given face.
*/
Cube::COLOR Cube::getSticker(LOCATION l)
{
	return (COLOR)(stickers[(uint8_t)l.face] >> ((7 - l.idx) * 8));
}

/**
* Return the location of the sticker adjacent to the given location.
*
* If a non-edge location is supplied, the second return value is set
* to false, otherwise it is true indicating a successful execution.
*/
std::pair<Cube::LOCATION, bool> Cube::getAdjacentEdge(LOCATION l)
{
	// fail if provided location isn't an edge
	if ((getFace(l.face) & edgesMask) == 0)
		return std::make_pair(LOCATION({ (FACE)0, 0 }), false);

	switch (l.face)
	{
	case (FACE::UP):
		switch (l.idx)
		{
		case 1:
			return std::make_pair(LOCATION({ FACE::BACK, 1 }), true);
		case 3:
			return std::make_pair(LOCATION({ FACE::RIGHT, 1 }), true);
		case 5:
			return std::make_pair(LOCATION({ FACE::FRONT , 1 }), true);
		case 7:
			return std::make_pair(LOCATION({ FACE::LEFT, 1 }), true);
		default:
			return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
		}
	case (FACE::DOWN):
		switch (l.idx)
		{
		case 1:
			return std::make_pair(LOCATION({ FACE::FRONT, 5 }), true);
		case 3:
			return std::make_pair(LOCATION({ FACE::RIGHT, 5 }), true);
		case 5:
			return std::make_pair(LOCATION({ FACE::BACK , 5 }), true);
		case 7:
			return std::make_pair(LOCATION({ FACE::LEFT, 5 }), true);
		default:
			return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
		}
	case (FACE::FRONT):
		switch (l.idx)
		{
		case 1:
			return std::make_pair(LOCATION({ FACE::UP, 5 }), true);
		case 3:
			return std::make_pair(LOCATION({ FACE::RIGHT, 7 }), true);
		case 5:
			return std::make_pair(LOCATION({ FACE::DOWN , 1 }), true);
		case 7:
			return std::make_pair(LOCATION({ FACE::LEFT, 3 }), true);
		default:
			return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
		}
	case (FACE::BACK):
		switch (l.idx)
		{
		case 1:
			return std::make_pair(LOCATION({ FACE::UP, 1 }), true);
		case 3:
			return std::make_pair(LOCATION({ FACE::LEFT, 7 }), true);
		case 5:
			return std::make_pair(LOCATION({ FACE::DOWN , 5 }), true);
		case 7:
			return std::make_pair(LOCATION({ FACE::RIGHT, 3 }), true);
		default:
			return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
		}
	case (FACE::RIGHT):
		switch (l.idx)
		{
		case 1:
			return std::make_pair(LOCATION({ FACE::UP, 3 }), true);
		case 3:
			return std::make_pair(LOCATION({ FACE::BACK, 7 }), true);
		case 5:
			return std::make_pair(LOCATION({ FACE::DOWN , 3 }), true);
		case 7:
			return std::make_pair(LOCATION({ FACE::FRONT, 3 }), true);
		default:
			return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
		}
	case (FACE::LEFT):
		switch (l.idx)
		{
		case 1:
			return std::make_pair(LOCATION({ FACE::UP, 7 }), true);
		case 3:
			return std::make_pair(LOCATION({ FACE::FRONT, 7 }), true);
		case 5:
			return std::make_pair(LOCATION({ FACE::DOWN , 7 }), true);
		case 7:
			return std::make_pair(LOCATION({ FACE::BACK, 3 }), true);
		default:
			return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
		}
	default:
		return std::make_pair(LOCATION({ (FACE)0, 0 }), false);
	}
}

/**
* Return the row of three sticker colors on the requested face.
*
* The color data will always be shifted to be in the lowest order bits.
*
* Only UP, DOWN, RIGHT, and LEFT are allowed for the row parameter,
* anything else will return 0.
*/
uint64_t Cube::getRow(FACE f, FACE row)
{
	uint64_t face = getFace(f);
	switch (row)
	{
	case FACE::UP:
		return (face & upMask) >> 40;
	case FACE::DOWN:
		return (face & downMask) >> 8;
	case FACE::RIGHT:
		return (face & rightMask) >> 24;
	case FACE::LEFT:
		return rotateLeft((face & leftMask), 8);
	default:
		return 0;
	}
}

/**
* Set the given face to the given value.
*/
void Cube::setFace(FACE f, uint64_t value)
{
	stickers[(uint8_t)f] = value;
}

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
* Perform every move present in the string of moves.
* No spaces are allowed in the input string.
*/
void Cube::readMoves(const std::string& moves)
{
	uint8_t moveIdx = 0;
	while (moveIdx < moves.length())
	{
		if (moveIdx < moves.length() - 1 && (moves[moveIdx + 1] == '\'' || moves[moveIdx + 1] == '2'))
		{
			readMove(moves.substr(moveIdx, 2));
			moveIdx += 2;
		}
		else
		{
			readMove(moves.substr(moveIdx, 1));
			moveIdx += 1;
		}
	}
}

/**
* Perform the single move represented by the given string.
*/
void Cube::readMove(const std::string& move)
{
	if (move == "U")
		u();
	else if (move == "U\'")
		uPrime();
	else if (move == "D")
		d();
	else if (move == "D\'")
		dPrime();
	else if (move == "F")
		f();
	else if (move == "F\'")
		fPrime();
	else if (move == "B")
		b();
	else if (move == "B\'")
		bPrime();
	else if (move == "R")
		r();
	else if (move == "R\'")
		rPrime();
	else if (move == "L")
		l();
	else if (move == "L\'")
		lPrime();
	else if (move == "M")
		m();
	else if (move == "M\'")
		mPrime();
	else if (move == "E")
		e();
	else if (move == "E\'")
		ePrime();
	else if (move == "S")
		s();
	else if (move == "S\'")
		sPrime();
	else if (move == "X")
		x();
	else if (move == "X\'")
		xPrime();
	else if (move == "Y")
		y();
	else if (move == "Y\'")
		yPrime();
	else if (move == "Z")
		z();
	else if (move == "Z\'")
		zPrime();
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
	setFace(FACE::FRONT, rotateRight(getFace(FACE::FRONT), 16));

	// turn the adjacent stickers on the up, right, bottom, and left faces 
	uint64_t toSave = getFace(FACE::UP) & downMask;
	setFace(FACE::UP, (getFace(FACE::UP) & ~downMask) | rotateLeft(getFace(FACE::RIGHT) & leftMask, 16));
	setFace(FACE::RIGHT, (getFace(FACE::RIGHT) & ~leftMask) | rotateLeft(getFace(FACE::DOWN) & upMask, 16));
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~upMask) | rotateLeft(getFace(FACE::LEFT) & rightMask, 16));
	setFace(FACE::LEFT, (getFace(FACE::LEFT) & ~rightMask) | (toSave << 16));
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
	setFace(FACE::FRONT, (getFace(FACE::FRONT) & ~leftMask) | getFace(FACE::DOWN) & leftMask);
	setFace(FACE::DOWN, (getFace(FACE::DOWN) & ~leftMask) | rotateRight(getFace(FACE::BACK) & rightMask, 32));
	setFace(FACE::BACK, (getFace(FACE::BACK) & ~rightMask) | rotateRight(toSave, 32));
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

	// print first row of left, front, right, and back faces, each separated by a space
	std::cout << "\n" << getColorChar(getSticker({ FACE::LEFT, 0 })) << getColorChar(getSticker({ FACE::LEFT, 1 })) << getColorChar(getSticker({ FACE::LEFT, 2 }));
	std::cout << " " << getColorChar(getSticker({ FACE::FRONT, 0 })) << getColorChar(getSticker({ FACE::FRONT, 1 })) << getColorChar(getSticker({ FACE::FRONT, 2 }));
	std::cout << " " << getColorChar(getSticker({ FACE::RIGHT, 0 })) << getColorChar(getSticker({ FACE::RIGHT, 1 })) << getColorChar(getSticker({ FACE::RIGHT, 2 }));
	std::cout << " " << getColorChar(getSticker({ FACE::BACK, 0 })) << getColorChar(getSticker({ FACE::BACK, 1 })) << getColorChar(getSticker({ FACE::BACK, 2 })) << std::endl;

	// print second row of left, center, right, and back faces, each separated by a space
	std::cout << getColorChar(getSticker({ FACE::LEFT, 7 })) << getColorChar(getCenter(FACE::LEFT)) << getColorChar(getSticker({ FACE::LEFT, 3 }));
	std::cout << " " << getColorChar(getSticker({ FACE::FRONT, 7 })) << getColorChar(getCenter(FACE::FRONT)) << getColorChar(getSticker({ FACE::FRONT, 3 }));
	std::cout << " " << getColorChar(getSticker({ FACE::RIGHT, 7 })) << getColorChar(getCenter(FACE::RIGHT)) << getColorChar(getSticker({ FACE::RIGHT, 3 }));
	std::cout << " " << getColorChar(getSticker({ FACE::BACK, 7 })) << getColorChar(getCenter(FACE::BACK)) << getColorChar(getSticker({ FACE::BACK, 3 })) << std::endl;

	// print third row of left, center, right, and back faces, each separated by a space
	std::cout << getColorChar(getSticker({ FACE::LEFT, 6 })) << getColorChar(getSticker({ FACE::LEFT, 5 })) << getColorChar(getSticker({ FACE::LEFT, 4 }));
	std::cout << " " << getColorChar(getSticker({ FACE::FRONT, 6 })) << getColorChar(getSticker({ FACE::FRONT, 5 })) << getColorChar(getSticker({ FACE::FRONT, 4 }));
	std::cout << " " << getColorChar(getSticker({ FACE::RIGHT, 6 })) << getColorChar(getSticker({ FACE::RIGHT, 5 })) << getColorChar(getSticker({ FACE::RIGHT, 4 }));
	std::cout << " " << getColorChar(getSticker({ FACE::BACK, 6 })) << getColorChar(getSticker({ FACE::BACK, 5 })) << getColorChar(getSticker({ FACE::BACK, 4 })) << "\n\n";

	// print the down face indented by 4 spaces
	std::cout << "    " << getColorChar(getSticker({ FACE::DOWN, 0 })) << getColorChar(getSticker({ FACE::DOWN, 1 })) << getColorChar(getSticker({ FACE::DOWN, 2 })) << std::endl;
	std::cout << "    " << getColorChar(getSticker({ FACE::DOWN, 7 })) << getColorChar(getCenter(FACE::DOWN)) << getColorChar(getSticker({ FACE::DOWN, 3 })) << std::endl;
	std::cout << "    " << getColorChar(getSticker({ FACE::DOWN, 6 })) << getColorChar(getSticker({ FACE::DOWN, 5 })) << getColorChar(getSticker({ FACE::DOWN, 4 })) << "\n\n";
}
