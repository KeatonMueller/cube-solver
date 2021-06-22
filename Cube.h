#ifndef CUBE_H
#define CUBE_H 

#include <stdint.h>
#include <string>
#include <utility>

class Cube
{
	/**
	* This class stores the complete state of a Rubik's Cube.
	*
	* Each face has 9 stickers, but since center pieces
	* don't move, we only need to store of them.
	*
	* Because there are only 6 possible colors a sticker can
	* be, the color can be encoded in 8 bits. Therefore, to store
	* all 8 colors for one particular face, you can store it in
	* a single 64-bit integer.
	*
	* Therefore, you can store the complete state in 6 64-bit
	* integers, one for each face.
	*
	* However, if we want to allow wide turns or slice moves, then
	* the location of the centers should be allowed to change. We
	* can therefore store the color of the six centers using one more
	* 64-bit integer, for a total of 7 64-bit integers to store the
	* entire cube state.
	*/

public:
	/**
	* 8-bit enum for each face
	*/
	enum class FACE : uint8_t
	{
		UP,
		DOWN,
		FRONT,
		BACK,
		RIGHT,
		LEFT
	};

	/**
	* 8-bit enum for each color
	*/
	enum class COLOR : uint8_t
	{
		WHITE,
		YELLOW,
		RED,
		ORANGE,
		BLUE,
		GREEN
	};

	/**
	* Struct to store the location of a non-center sticker
	*/
	struct LOCATION
	{
		FACE face;
		uint8_t idx;
	};

	Cube();

	COLOR getCenter(FACE face);
	COLOR getSticker(LOCATION loc);
	std::pair<LOCATION, bool> getAdjacentEdge(LOCATION loc);

	std::string move(FACE face, const std::string& type = "");
	bool isEdgeSolved(LOCATION l);

	void printLocation(LOCATION loc);

	/**
	* Read and execute a sequence of moves.
	*/
	void readMoves(const std::string& moves);

	/**
	* Functions for outer turns.
	*/
	void u();
	void uPrime();
	void d();
	void dPrime();
	void f();
	void fPrime();
	void b();
	void bPrime();
	void r();
	void rPrime();
	void l();
	void lPrime();
	/**
	* Functions for slice turns.
	*/
	void m();
	void mPrime();
	void s();
	void sPrime();
	void e();
	void ePrime();
	/**
	* Functions for cube rotations.
	*/
	void x();
	void xPrime();
	void y();
	void yPrime();
	void z();
	void zPrime();

	/**
	* Print to the console in a readable format
	*/
	void print();

private:
	/**
	* Array of 7 64-bit integers. The first six integers correspond to
	* the 8 outer stickers of each face, in the following order: Up,
	* Down, Front, Back, Right, Left.
	*
	* The seventh integer corresponds to the colors of the centers in
	* the same order.
	*
	* The order of the colors for each individual face is starting in the top left
	* of the face and circling clockwise.
	*
	* For example, the color sequence W, Y, R, O, B, G, W, Y
	* would be arranged on a face like this:
	*
	* WYR
	* Y O
	* WGB
	*/
	uint64_t stickers[7];

	/**
	* Bit masks for selecting specific rows of stickers
	*/
	uint64_t upMask = 0xffffff0000000000;
	uint64_t rightMask = 0x0000ffffff000000;
	uint64_t downMask = 0x00000000ffffff00;
	uint64_t leftMask = 0xff0000000000ffff;
	uint64_t middleColMask = 0x00ff000000ff0000;
	uint64_t middleRowMask = 0x000000ff000000ff;

	/**
	* Bit masks for selecting certain types of stickers
	*/
	uint64_t edgesMask = 0x00ff00ff00ff00ff;
	uint64_t cornersMask = 0xff00ff00ff00ff00;

	/**
	* Helper functions for selecting specific stickers
	*/
	char getColorChar(COLOR c);
	uint64_t getFace(FACE f);

	/**
	* Helper functions for updating the cube's values
	*/
	void setFace(FACE f, uint64_t value);
	void setCenter(FACE f, COLOR c);

	/**
	* Read and execute a single move.
	*/
	void readMove(const std::string& move);
};

#endif
