#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>
#include <string>

class Move
{
public:
	/**
	* 8-bit enum for the pieces to move.
	*/
	enum class PIECES : uint8_t
	{
		UP,
		DOWN,
		FRONT,
		BACK,
		RIGHT,
		LEFT,
		FRONT_WIDE,
		BACK_WIDE,
		UP_WIDE,
		DOWN_WIDE,
		RIGHT_WIDE,
		LEFT_WIDE,
		M_SLICE,
		E_SLICE,
		S_SLICE,
		X,
		Y,
		Z
	};

	/**
	* 8-bit enum for types of moves.
	*/
	enum class TYPE : uint8_t
	{
		NORMAL,
		PRIME,
		DOUBLE,
		NO_MOVE
	};

	/**
	* Create a new move.
	*/
	Move(PIECES newPieces, TYPE newType);

	/**
	* The pieces and type of this move.
	*/
	PIECES pieces;
	TYPE type;

	/**
	* Check if this move can merge with another.
	*/
	bool canMergeWith(const Move& move);
	/**
	* Merge this move with another.
	*/
	Move merge(const Move& move);

	/**
	* Get a string representation of this move.
	*/
	std::string toString();
};

#endif
