#include "Move.h"

/**
* Create a new move of the given pieces and type.
*/
Move::Move(PIECES newPieces, TYPE newType)
{
	pieces = newPieces;
	type = newType;
}

/**
* Check if this move can merge with another.
*/
bool Move::canMergeWith(const Move& move)
{
	// can't merge if either are NO_MOVE
	if (type == TYPE::NO_MOVE || move.type == TYPE::NO_MOVE)
		return false;
	// can only merge if the pieces are the same
	if (move.pieces != pieces)
		return false;
	return true;
}

/**
* Merge this move with another.
*
* Return a new move which is the merged version of the two.
*
* The new move's type is set to NO_MOVE if the merge results
* in an elimination.
*/
Move Move::merge(const Move& move)
{
	TYPE newType = type;
	switch (type)
	{
	case TYPE::NORMAL:
		switch (move.type)
		{
		case TYPE::NORMAL:
			// normal + normal = double
			newType = TYPE::DOUBLE;
			break;
		case TYPE::PRIME:
			// normal + prime = no move
			newType = TYPE::NO_MOVE;
			break;
		case TYPE::DOUBLE:
			// normal + double = prime
			newType = TYPE::PRIME;
			break;
		default:
			break;
		}
		break;
	case TYPE::PRIME:
		switch (move.type)
		{
		case TYPE::NORMAL:
			// prime + normal = no move
			newType = TYPE::NO_MOVE;
			break;
		case TYPE::PRIME:
			// prime + prime = double
			newType = TYPE::DOUBLE;
			break;
		case TYPE::DOUBLE:
			// prime + double = normal
			newType = TYPE::NORMAL;
			break;
		default:
			break;
		}
		break;
	case TYPE::DOUBLE:
		switch (move.type)
		{
		case TYPE::NORMAL:
			// double + normal = prime
			newType = TYPE::PRIME;
			break;
		case TYPE::PRIME:
			// double + prime = normal
			newType = TYPE::NORMAL;
			break;
		case TYPE::DOUBLE:
			// double + double = no move
			newType = TYPE::NO_MOVE;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return Move(pieces, newType);
}

/**
* Get a string representation of this move.
*/
std::string Move::toString()
{
	std::string value = "";
	switch (pieces)
	{
	case PIECES::UP:
		value += "U";
		break;
	case PIECES::DOWN:
		value += "D";
		break;
	case PIECES::FRONT:
		value += "F";
		break;
	case PIECES::BACK:
		value += "B";
		break;
	case PIECES::RIGHT:
		value += "R";
		break;
	case PIECES::LEFT:
		value += "L";
		break;
	case PIECES::UP_WIDE:
		value += "u";
		break;
	case PIECES::DOWN_WIDE:
		value += "d";
		break;
	case PIECES::FRONT_WIDE:
		value += "f";
		break;
	case PIECES::BACK_WIDE:
		value += "b";
		break;
	case PIECES::RIGHT_WIDE:
		value += "r";
		break;
	case PIECES::LEFT_WIDE:
		value += "l";
		break;
	case PIECES::M_SLICE:
		value += "M";
		break;
	case PIECES::E_SLICE:
		value += "E";
		break;
	case PIECES::S_SLICE:
		value += "S";
		break;
	case PIECES::X:
		value += "x";
		break;
	case PIECES::Y:
		value += "y";
		break;
	case PIECES::Z:
		value += "z";
		break;
	}

	switch (type)
	{
	case TYPE::PRIME:
		value += "\'";
		break;
	case TYPE::DOUBLE:
		value += "2";
		break;
	default:
		break;
	}

	return value;
}