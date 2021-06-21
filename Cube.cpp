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
Cube::COLOR Cube::getSticker(FACE f, uint8_t idx)
{
	return (COLOR)(stickers[(uint8_t)f] >> ((7 - idx) * 8));
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
		return (face & 0xffffff0000000000) >> 40;
	case FACE::DOWN:
		return (face & 0xffffff00) >> 8;
	case FACE::RIGHT:
		return (face & 0xffffff000000) >> 24;
	case FACE::LEFT:
		return rotateLeft((face & 0xff0000000000ffff), 8);
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
	std::cout << "    " << getColorChar(getSticker(FACE::UP, 0)) << getColorChar(getSticker(FACE::UP, 1)) << getColorChar(getSticker(FACE::UP, 2)) << std::endl;
	std::cout << "    " << getColorChar(getSticker(FACE::UP, 7)) << getColorChar(getCenter(FACE::UP)) << getColorChar(getSticker(FACE::UP, 3)) << std::endl;
	std::cout << "    " << getColorChar(getSticker(FACE::UP, 6)) << getColorChar(getSticker(FACE::UP, 5)) << getColorChar(getSticker(FACE::UP, 4)) << std::endl;

	// print first row of left, front, right, and back faces, each separated by a space
	std::cout << "\n" << getColorChar(getSticker(FACE::LEFT, 0)) << getColorChar(getSticker(FACE::LEFT, 1)) << getColorChar(getSticker(FACE::LEFT, 2));
	std::cout << " " << getColorChar(getSticker(FACE::FRONT, 0)) << getColorChar(getSticker(FACE::FRONT, 1)) << getColorChar(getSticker(FACE::FRONT, 2));
	std::cout << " " << getColorChar(getSticker(FACE::RIGHT, 0)) << getColorChar(getSticker(FACE::RIGHT, 1)) << getColorChar(getSticker(FACE::RIGHT, 2));
	std::cout << " " << getColorChar(getSticker(FACE::BACK, 0)) << getColorChar(getSticker(FACE::BACK, 1)) << getColorChar(getSticker(FACE::BACK, 2)) << std::endl;

	// print second row of left, center, right, and back faces, each separated by a space
	std::cout << getColorChar(getSticker(FACE::LEFT, 7)) << getColorChar(getCenter(FACE::LEFT)) << getColorChar(getSticker(FACE::LEFT, 3));
	std::cout << " " << getColorChar(getSticker(FACE::FRONT, 7)) << getColorChar(getCenter(FACE::FRONT)) << getColorChar(getSticker(FACE::FRONT, 3));
	std::cout << " " << getColorChar(getSticker(FACE::RIGHT, 7)) << getColorChar(getCenter(FACE::RIGHT)) << getColorChar(getSticker(FACE::RIGHT, 3));
	std::cout << " " << getColorChar(getSticker(FACE::BACK, 7)) << getColorChar(getCenter(FACE::BACK)) << getColorChar(getSticker(FACE::BACK, 3)) << std::endl;

	// print third row of left, center, right, and back faces, each separated by a space
	std::cout << getColorChar(getSticker(FACE::LEFT, 6)) << getColorChar(getSticker(FACE::LEFT, 5)) << getColorChar(getSticker(FACE::LEFT, 4));
	std::cout << " " << getColorChar(getSticker(FACE::FRONT, 6)) << getColorChar(getSticker(FACE::FRONT, 5)) << getColorChar(getSticker(FACE::FRONT, 4));
	std::cout << " " << getColorChar(getSticker(FACE::RIGHT, 6)) << getColorChar(getSticker(FACE::RIGHT, 5)) << getColorChar(getSticker(FACE::RIGHT, 4));
	std::cout << " " << getColorChar(getSticker(FACE::BACK, 6)) << getColorChar(getSticker(FACE::BACK, 5)) << getColorChar(getSticker(FACE::BACK, 4)) << "\n\n";

	// print the down face indented by 4 spaces
	std::cout << "    " << getColorChar(getSticker(FACE::DOWN, 0)) << getColorChar(getSticker(FACE::DOWN, 1)) << getColorChar(getSticker(FACE::DOWN, 2)) << std::endl;
	std::cout << "    " << getColorChar(getSticker(FACE::DOWN, 7)) << getColorChar(getCenter(FACE::DOWN)) << getColorChar(getSticker(FACE::DOWN, 3)) << std::endl;
	std::cout << "    " << getColorChar(getSticker(FACE::DOWN, 6)) << getColorChar(getSticker(FACE::DOWN, 5)) << getColorChar(getSticker(FACE::DOWN, 4)) << "\n\n";
}
