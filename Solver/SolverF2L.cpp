#include <vector>

#include <iostream> // TODO: remove
#include "SolverF2L.h"

/**
* Find the edge piece that matches with the given corner piece.
*
* Returns the edge piece that matches the sticker colors of
* the two stickers adjacent to the given corner location.
*/
LOCATION findMatchingEdge(Cube& cube, LOCATION cornerLoc)
{
	// get the adjacent stickers of the corner piece
	std::pair<LOCATION, LOCATION> cornerAdjLocs = cube.getAdjacentCorner(cornerLoc);
	COLOR cornerAdjSticker0 = cube.getSticker(cornerAdjLocs.first);
	COLOR cornerAdjSticker1 = cube.getSticker(cornerAdjLocs.second);

	// search every edge location
	LOCATION edgeLoc;
	COLOR edgeSticker0, edgeSticker1;
	for (uint8_t edgeFace = 0; edgeFace < 6; edgeFace++)
	{
		// skip these faces to prevent double counting
		if ((FACE)edgeFace == FACE::DOWN || (FACE)edgeFace == FACE::FRONT || (FACE)edgeFace == FACE::BACK)
			continue;

		for (uint8_t edgeIdx = 1; edgeIdx < 8; edgeIdx += 2)
		{
			// ignore idx 3 and 7 on up face to prevent double counting
			if ((FACE)edgeFace == FACE::UP && (edgeIdx == 3 || edgeIdx == 7))
				continue;

			// ignore idx 5 on right and left faces to prevent double counting
			if (((FACE)edgeFace == FACE::RIGHT || (FACE)edgeFace == FACE::LEFT) && edgeIdx == 5)
				continue;

			edgeLoc = { (FACE)edgeFace, edgeIdx };
			edgeSticker0 = cube.getSticker(edgeLoc);
			edgeSticker1 = cube.getSticker(cube.getAdjacentEdge(edgeLoc));

			// check if edge matches corner
			if ((cornerAdjSticker0 == edgeSticker0 && cornerAdjSticker1 == edgeSticker1) ||
				(cornerAdjSticker0 == edgeSticker1 && cornerAdjSticker1 == edgeSticker0))
			{
				return edgeLoc;
			}
		}
	}
	// not possible, return anything to satisfy compiler
	return { (FACE)0, 0 };
}

/**
* Find an unsolved F2L pair on the given cube.
*
* Return it as a <pair, bool> pair. The bool indicates
* if an unsolved pair was able to be located. The pair
* is the location of the corner and edge piece, in that
* order, of the unsolved F2L pair.
*
* Cube is assumed to be oriented with cross color down.
*/
std::pair<std::pair<LOCATION, LOCATION>, bool> findUnsolvedF2LPair(Cube& cube, COLOR crossColor)
{
	// find an unsolved F2L pair
	LOCATION cornerLoc, edgeLoc;
	// search every corner location
	for (uint8_t cornerFace = 0; cornerFace < 6; cornerFace++)
	{
		for (uint8_t cornerIdx = 0; cornerIdx < 8; cornerIdx += 2)
		{
			// if this sticker is the cross color, get corresponding edge
			cornerLoc = { (FACE)cornerFace, cornerIdx };
			if (cube.getSticker(cornerLoc) == crossColor)
			{
				edgeLoc = findMatchingEdge(cube, cornerLoc);

				// return if this pair is unsolved
				if (!cube.isPieceSolved(cornerLoc) || !cube.isPieceSolved(edgeLoc))
					return std::make_pair(std::make_pair(cornerLoc, edgeLoc), true);
			}
		}
	}
	// F2L is solved, return false
	return std::make_pair(std::make_pair(cornerLoc, edgeLoc), false);
}

/**
* Determine if the F2L slot under the given idx
* (idx corresponds to the UP face) is solved or not.
*/
bool isSlotSolved(Cube& cube, uint8_t idx)
{
	if (idx == 0)
		return cube.isPieceSolved({ FACE::LEFT, 6 }) && cube.isPieceSolved({ FACE::LEFT, 7 });
	else if (idx == 2)
		return cube.isPieceSolved({ FACE::RIGHT, 3 }) && cube.isPieceSolved({ FACE::RIGHT, 4 });
	else if (idx == 4)
		return cube.isPieceSolved({ FACE::RIGHT, 6 }) && cube.isPieceSolved({ FACE::RIGHT, 7 });
	else if (idx == 6)
		return cube.isPieceSolved({ FACE::LEFT, 3 }) && cube.isPieceSolved({ FACE::LEFT, 4 });
	// invalid argument
	return false;
}

/**
* Determine if the given F2L pair is paired up.
* In other words, if the pieces are adjacent with the
* colors aligned.
*/
bool isF2LPairPaired(Cube& cube, LOCATION cornerLoc, LOCATION edgeLoc)
{
	std::pair<LOCATION, LOCATION> cornerAdjLocs = cube.getAdjacentCorner(cornerLoc);
	LOCATION edgeAdjLoc = cube.getAdjacentEdge(edgeLoc);

	// check that the same faces have the same colors
	if (cornerAdjLocs.first.face == edgeLoc.face && cornerAdjLocs.second.face == edgeAdjLoc.face)
	{
		if (cube.getSticker(cornerAdjLocs.first) != cube.getSticker(edgeLoc))
			return false;
		if (cube.getSticker(cornerAdjLocs.second) != cube.getSticker(edgeAdjLoc))
			return false;
		return true;
	}
	else if (cornerAdjLocs.second.face == edgeLoc.face && cornerAdjLocs.first.face == edgeAdjLoc.face)
	{

		if (cube.getSticker(cornerAdjLocs.second) != cube.getSticker(edgeLoc))
			return false;
		if (cube.getSticker(cornerAdjLocs.first) != cube.getSticker(edgeAdjLoc))
			return false;
		return true;
	}
	// pieces aren't on same faces; they're not paired up
	return false;
}

/**
* Locate the F2L pair with the given colors.
*
* Return a pair of locations where the first corresponds
* to the cross color sticker of the corner piece, and the
* second corresponds to any sticker of the edge piece.
*/
std::pair<LOCATION, LOCATION> locateF2LPair(Cube& cube, COLOR pairColor0, COLOR pairColor1)
{
	std::pair<LOCATION, LOCATION> pair;
	COLOR crossColor = cube.getCenter(FACE::DOWN);
	LOCATION loc = { FACE::UP, 0 };
	std::pair<LOCATION, LOCATION> cornerAdjLocs;
	COLOR color0, color1;
	// search UP and DOWN faces to cover all corners
	for (uint8_t face = 0; face < 2; face++)
	{
		loc.face = (FACE)face;
		for (uint8_t idx = 0; idx < 8; idx += 2)
		{
			loc.idx = idx;
			// check that it contains both colors and the cross color
			cornerAdjLocs = cube.getAdjacentCorner(loc);
			color0 = cube.getSticker(cornerAdjLocs.first);
			color1 = cube.getSticker(cornerAdjLocs.second);

			if (cube.getSticker(loc) == crossColor)
			{
				if ((pairColor0 == color0 && pairColor1 == color1) ||
					(pairColor1 == color0 && pairColor0 == color1))
				{
					pair.first = loc;
				}
			}
			else if (color0 == crossColor)
			{
				if ((pairColor0 == cube.getSticker(loc) && pairColor1 == color1) ||
					(pairColor1 == cube.getSticker(loc) && pairColor0 == color1))
				{
					pair.first = cornerAdjLocs.first;
				}
			}
			else if (color1 == crossColor)
			{
				if ((pairColor0 == color0 && pairColor1 == cube.getSticker(loc)) ||
					(pairColor1 == color0 && pairColor0 == cube.getSticker(loc)))
				{
					pair.first = cornerAdjLocs.second;
				}
			}
		}
	}
	// find the matching edge piece
	pair.second = findMatchingEdge(cube, pair.first);

	return pair;
}

/**
* Return the idx in the UP layer of the position
* directly above the given location.
*
* Location is assumed to be an edge location in the
* middle layer.
*/
uint8_t getAboveUpIdxEdge(LOCATION loc)
{
	switch (loc.face)
	{
	case FACE::FRONT:
		if (loc.idx == 3)
			return 4;
		else if (loc.idx == 7)
			return 6;
		break;
	case FACE::BACK:
		if (loc.idx == 3)
			return 0;
		else if (loc.idx == 7)
			return 2;
		break;
	case FACE::RIGHT:
		if (loc.idx == 3)
			return 2;
		else if (loc.idx == 7)
			return 4;
		break;
	case FACE::LEFT:
		if (loc.idx == 3)
			return 6;
		else if (loc.idx == 7)
			return 0;
		break;
	default:
		break;
	}
	// shouldn't be possible, return invalid idx
	return 8;
}

/**
* Get the idx in the UP layer for the corner piece
* that the given location is a part of.
*/
uint8_t getUpIdxCorner(LOCATION loc)
{
	switch (loc.face)
	{
	case FACE::UP:
		return loc.idx;
	case FACE::DOWN:
		if (loc.idx == 0)
			return 6;
		else if (loc.idx == 2)
			return 4;
		else if (loc.idx == 4)
			return 2;
		else if (loc.idx == 6)
			return 0;
		break;
	case FACE::FRONT:
		if (loc.idx == 0 || loc.idx == 6)
			return 6;
		else if (loc.idx == 2 || loc.idx == 4)
			return 4;
		break;
	case FACE::BACK:
		if (loc.idx == 0 || loc.idx == 6)
			return 2;
		else if (loc.idx == 2 || loc.idx == 4)
			return 0;
		break;
	case FACE::RIGHT:
		if (loc.idx == 0 || loc.idx == 6)
			return 4;
		else if (loc.idx == 2 || loc.idx == 4)
			return 2;
		break;
	case FACE::LEFT:
		if (loc.idx == 0 || loc.idx == 6)
			return 0;
		else if (loc.idx == 2 || loc.idx == 4)
			return 6;
		break;
	}
	// invalid argument, return invalid index
	return 8;
}

/**
* Ensure that both F2L pieces are in the top layer.
*
* When possible, make it so this step ends with the pair
* only requiring a three move insert to be solved.
*/
void bringF2LToTop(Cube& cube, LOCATION cornerLoc, LOCATION edgeLoc, std::vector<Move>& solution)
{
	LAYER cornerLayer = getLayer(cornerLoc);
	LAYER edgeLayer = getLayer(edgeLoc);
	LOCATION edgeAdjLoc = cube.getAdjacentEdge(edgeLoc);

	// both pieces in top layer, just return
	if (cornerLayer == LAYER::TOP && edgeLayer == LAYER::TOP)
		return;
	// corner in top, but edge is in the middle
	else if (cornerLayer == LAYER::TOP)
	{
		// if cross color is facing up
		if (cornerLoc.face == FACE::UP)
		{
			// check if aligning corner with edge would pair them up
			std::pair<LOCATION, LOCATION> cornerAdjLocs = cube.getAdjacentCorner(cornerLoc);
			COLOR leftCornerSticker = cornerAdjLocs.first.idx == 2 ?
				cube.getSticker(cornerAdjLocs.first) :
				cube.getSticker(cornerAdjLocs.second);
			COLOR leftEdgeSticker = edgeLoc.idx == 3 ?
				cube.getSticker(edgeLoc) :
				cube.getSticker(edgeAdjLoc);

			// if they align to form a pair
			if (leftCornerSticker == leftEdgeSticker)
			{
				// bring cross color piece over slot
				uint8_t targetIdx = getAboveUpIdxEdge(edgeLoc);
				if ((cornerLoc.idx + 2) % 8 == targetIdx)
					solution.push_back(cube.move(FACE::UP));
				else if ((cornerLoc.idx + 4) % 8 == targetIdx)
					solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
				else if ((cornerLoc.idx + 6) % 8 == targetIdx)
					solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

				// take out pair
				FACE rightFace = edgeLoc.idx == 7 ? edgeLoc.face : edgeAdjLoc.face;
				solution.push_back(cube.move(rightFace));
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
				solution.push_back(cube.move(rightFace, Move::TYPE::PRIME));
			}
			// otherwise
			else
			{
				// ensure cross color piece isn't in above slot or opposite slot
				if (cornerLoc.idx == getAboveUpIdxEdge(edgeLoc) || cornerLoc.idx == (getAboveUpIdxEdge(edgeLoc) + 4) % 8)
					solution.push_back(cube.move(FACE::UP));

				// bring edge into top layer
				FACE rightFace = edgeLoc.idx == 7 ? edgeLoc.face : edgeAdjLoc.face;
				solution.push_back(cube.move(rightFace));
				solution.push_back(cube.move(FACE::UP));
				solution.push_back(cube.move(rightFace, Move::TYPE::PRIME));
			}
		}
		// cross color is facing to the side
		else
		{
			// you want the cross color to share a face with the edge,
			// but not be directly over it
			uint8_t turns = 0;
			while (true)
			{
				// stop if conditions have been met
				if ((getAboveUpIdxEdge(edgeLoc) != getUpIdxCorner(cornerLoc)) &&
					(cornerLoc.face == edgeLoc.face || cornerLoc.face == edgeAdjLoc.face))
				{
					break;
				}
				// otherwise rotate up face
				else
				{
					cube.u();
					turns++;
					cornerLoc.face = cube.getAdjacentFace(cornerLoc.face, "y");
				}
			}
			// translate the number of turns needed
			if (turns == 1)
				solution.push_back(Move(Move::PIECES::UP, Move::TYPE::NORMAL));
			else if (turns == 2)
				solution.push_back(Move(Move::PIECES::UP, Move::TYPE::DOUBLE));
			else if (turns == 3)
				solution.push_back(Move(Move::PIECES::UP, Move::TYPE::PRIME));

			// remember the color facing up on the corner piece
			COLOR cornerUpColor = cube.getSticker({ FACE::UP, getUpIdxCorner(cornerLoc) });

			// if cross color sticker is on right side
			if (cornerLoc.idx == 0)
			{
				// bring edge to top layer
				solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));

				// if raised edge color matches
				if (cornerUpColor == cube.getSticker(cube.getAdjacentEdge({ cornerLoc.face, 1 })))
					solution.push_back(cube.move(FACE::UP));
				// if raised edge color doesn't match
				else
					solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

				// bring corner piece back to top layer
				solution.push_back(cube.move(cornerLoc.face));
			}
			// if cross color sticker is on left side
			else if (cornerLoc.idx == 2)
			{
				// bring edge to top layer
				solution.push_back(cube.move(cornerLoc.face));

				// if raised edge color matches
				if (cornerUpColor == cube.getSticker(cube.getAdjacentEdge({ cornerLoc.face, 1 })))
					solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
				// if raised edge color doesn't match
				else
					solution.push_back(cube.move(FACE::UP));

				// bring corner piece back to top layer
				solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));
			}
		}
	}
	// edge in top, but corner is in the bottom
	else if (edgeLayer == LAYER::TOP)
	{
		// get the edge color that's in the up layer
		COLOR edgeUpColor = edgeLoc.face == FACE::UP ?
			cube.getSticker(edgeLoc) :
			cube.getSticker(cube.getAdjacentEdge(edgeLoc));

		// if cross color facing down
		if (cornerLoc.face == FACE::DOWN)
		{
			// get stickers adjacent to corner
			std::pair<LOCATION, LOCATION> cornerAdjLocs = cube.getAdjacentCorner(cornerLoc);
			// determine which color is on the left
			COLOR leftColor;
			if (cornerAdjLocs.first.idx == 4)
				leftColor = cube.getSticker(cornerAdjLocs.first);
			else
				leftColor = cube.getSticker(cornerAdjLocs.second);

			// place edge in proper position
			uint8_t targetIdx;
			// if left color matches the edge facing up
			if (leftColor == edgeUpColor)
				// position to the back right
				targetIdx = (getUpIdxCorner(cornerLoc) + 5) % 8;
			// if right color matches the edge facing up
			else
				// position to the back left
				targetIdx = (getUpIdxCorner(cornerLoc) + 3) % 8;

			// move edge to target position
			uint8_t edgeIdx = edgeLoc.face == FACE::UP ? edgeLoc.idx : cube.getAdjacentEdge(edgeLoc).idx;
			if ((edgeIdx + 2) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP));
			else if ((edgeIdx + 4) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
			else if ((edgeIdx + 6) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

			// bring corner to top layer, pairing it with the edge
			if (leftColor == edgeUpColor)
			{
				FACE leftFace = cornerAdjLocs.first.idx == 4 ? cornerAdjLocs.first.face : cornerAdjLocs.second.face;
				solution.push_back(cube.move(leftFace, Move::TYPE::PRIME));
				solution.push_back(cube.move(FACE::UP));
				solution.push_back(cube.move(leftFace));
			}
			else
			{
				FACE rightFace = cornerAdjLocs.first.idx == 6 ? cornerAdjLocs.first.face : cornerAdjLocs.second.face;
				solution.push_back(cube.move(rightFace));
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
				solution.push_back(cube.move(rightFace, Move::TYPE::PRIME));
			}
		}
		// if cross color facing left
		else if (cornerLoc.idx == 4)
		{
			// get adjacent color not facing down
			COLOR cornerAdjColor = cube.getSticker({ cube.getAdjacentFace(cornerLoc.face, "yPrime"), 6 });

			uint8_t targetIdx;
			// if colors match
			if (cornerAdjColor == edgeUpColor)
				// position edge to the right of the slot
				targetIdx = (getUpIdxCorner(cornerLoc) + 7) % 8;
			// otherwise
			else
				// position edge to the left of the slot
				targetIdx = (getUpIdxCorner(cornerLoc) + 1) % 8;

			// move edge to target position
			uint8_t edgeIdx = edgeLoc.face == FACE::UP ? edgeLoc.idx : cube.getAdjacentEdge(edgeLoc).idx;
			if ((edgeIdx + 2) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP));
			else if ((edgeIdx + 4) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
			else if ((edgeIdx + 6) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

			// bring corner out of bottom layer
			solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));
			solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
			solution.push_back(cube.move(cornerLoc.face));
		}
		// if cross color facing right
		else if (cornerLoc.idx == 6)
		{
			// get adjacent color not facing down
			COLOR cornerAdjColor = cube.getSticker({ cube.getAdjacentFace(cornerLoc.face, "y"), 4 });

			uint8_t targetIdx;
			// if colors match
			if (cornerAdjColor == edgeUpColor)
				// position edge to the left of the slot
				targetIdx = (getUpIdxCorner(cornerLoc) + 1) % 8;
			// otherwise
			else
				// position edge to the right of the slot
				targetIdx = (getUpIdxCorner(cornerLoc) + 7) % 8;

			// move edge to target position
			uint8_t edgeIdx = edgeLoc.face == FACE::UP ? edgeLoc.idx : cube.getAdjacentEdge(edgeLoc).idx;
			if ((edgeIdx + 2) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP));
			else if ((edgeIdx + 4) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
			else if ((edgeIdx + 6) % 8 == targetIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

			// bring corner out of bottom layer
			solution.push_back(cube.move(cornerLoc.face));
			solution.push_back(cube.move(FACE::UP));
			solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));
		}
	}
	// corner in bottom and edge in middle
	else
	{
		// save colors
		COLOR color0 = cube.getSticker(edgeLoc);
		COLOR color1 = cube.getSticker(cube.getAdjacentEdge(edgeLoc));
		// bring edge to the top. if corner is in same slot, this
		// splits them. if it wasn't that's why we recurse
		FACE rightFace = edgeLoc.idx == 7 ? edgeLoc.face : edgeAdjLoc.face;
		solution.push_back(cube.move(rightFace));
		solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
		solution.push_back(cube.move(rightFace, Move::TYPE::PRIME));

		std::pair<LOCATION, LOCATION> pairLoc = locateF2LPair(cube, color0, color1);
		bringF2LToTop(cube, pairLoc.first, pairLoc.second, solution);
	}
}

/**
* Rotate the UP face until the piece at the given idx is
* above an unsolved slot.
*
* Return the number of 90 degree clockwise turns necessary
* to make the piece over an unsolved slot.
*
* If no unsolved slots are present, return 4.
*/
uint8_t moveToUnsolvedSlot(Cube& cube, uint8_t idx, std::vector<Move>& solution)
{
	if (!isSlotSolved(cube, idx))
		return 0;

	if (!isSlotSolved(cube, (idx + 2) % 8))
	{
		solution.push_back(cube.move(FACE::UP));
		return 1;
	}

	if (!isSlotSolved(cube, (idx + 4) % 8))
	{
		solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
		return 2;
	}

	if (!isSlotSolved(cube, (idx + 6) % 8))
	{
		solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
		return 3;
	}

	// no unsolved slot present
	return 4;
}

/**
* Find the idx in the UP face directly above the F2L slot
* corresponding to the two given colors.
*
* If no suitable F2L slot is found, return 8.
*/
uint8_t findF2LSlot(Cube& cube, COLOR color0, COLOR color1)
{
	std::pair<LOCATION, LOCATION> cornerAdjLocs;
	COLOR cornerColor0, cornerColor1;
	for (uint8_t targetIdx = 0; targetIdx < 8; targetIdx += 2)
	{
		cornerAdjLocs = cube.getAdjacentCorner({ FACE::UP, targetIdx });
		cornerColor0 = cube.getCenter(cornerAdjLocs.first.face);
		cornerColor1 = cube.getCenter(cornerAdjLocs.second.face);

		if ((cornerColor0 == color0 && cornerColor1 == color1) ||
			(cornerColor1 == color0 && cornerColor0 == color1))
			return targetIdx;
	}
	return 8;
}

/**
* Rotate the UP face until the piece at the given idx is
* positioned above the F2L slot indicated by the given colors.
*
* Return the new idx it ends up in.
*
* If the given colors do not correspond to a valid F2L slot,
* return 8.
*/
uint8_t moveToSlot(Cube& cube, uint8_t idx, COLOR color0, COLOR color1, std::vector<Move>& solution)
{
	// find the idx of the target slot
	uint8_t targetIdx = findF2LSlot(cube, color0, color1);

	// rotate UP until idx reaches targetIdx
	if ((idx + 2) % 8 == targetIdx)
		solution.push_back(cube.move(FACE::UP));
	else if ((idx + 4) % 8 == targetIdx)
		solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
	else if ((idx + 6) % 8 == targetIdx)
		solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

	return targetIdx;
}

/**
* Make sure the given F2L pieces are split, aka not adjacent.
*
* Ensures no solved slots are disturbed in the process of
* splitting the pair.
*
* If the given pieces have the cross color facing to the side,
* with different colors facing up, they are split in such a way
* that sets up a three move insert.
*/
void splitF2LPair(Cube& cube, LOCATION cornerLoc, LOCATION edgeLoc, std::vector<Move>& solution)
{
	uint8_t cornerUpIdx = getUpIdxCorner(cornerLoc);
	uint8_t edgeUpIdx = edgeLoc.face == FACE::UP ? edgeLoc.idx : cube.getAdjacentEdge(edgeLoc).idx;

	// if edge is to the left of the corner
	if ((cornerUpIdx + 1) % 8 == edgeUpIdx)
	{
		// need slot at cornerUpIdx - 2 to be unsolved
		uint8_t turnsNeeded = moveToUnsolvedSlot(cube, (cornerUpIdx + 6) % 8, solution);
		cornerUpIdx = (cornerUpIdx + (2 * turnsNeeded)) % 8;

		// find the right face
		std::pair<LOCATION, LOCATION> cornerAdjLocs = cube.getAdjacentCorner({ FACE::UP, cornerUpIdx });
		FACE rightFace = cornerAdjLocs.first.idx == 0 ? cornerAdjLocs.first.face : cornerAdjLocs.second.face;

		// split up pair
		solution.push_back(cube.move(rightFace, Move::TYPE::PRIME));

		// if the cross color is facing right, perform a U2 to potentially set up a three move insert
		if (cornerLoc.idx == 0)
			solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
		// otherwise perform a U, whichs sets it up if cross color is facing left
		else
			solution.push_back(cube.move(FACE::UP));

		solution.push_back(cube.move(rightFace));
	}
	// if edge is to the right of the corner
	else if ((edgeUpIdx + 1) % 8 == cornerUpIdx)
	{
		// need slot at cornerUpIdx + 2 to be unsolved
		uint8_t turnsNeeded = moveToUnsolvedSlot(cube, (cornerUpIdx + 2) % 8, solution);
		cornerUpIdx = (cornerUpIdx + (2 * turnsNeeded)) % 8;

		// find the left face
		std::pair<LOCATION, LOCATION> cornerAdjLocs = cube.getAdjacentCorner({ FACE::UP, cornerUpIdx });
		FACE leftFace = cornerAdjLocs.first.idx == 2 ? cornerAdjLocs.first.face : cornerAdjLocs.second.face;

		// split up pair
		solution.push_back(cube.move(leftFace));

		// if the cross color is facing left, perform a U2 to potentially set up a three move insert
		if (cornerLoc.idx == 2)
			solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
		// otherwise perform a U', whichs sets it up if cross color is facing right
		else
			solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

		solution.push_back(cube.move(leftFace, Move::TYPE::PRIME));
	}
}

/**
* Prepare the given F2L pair to be inserted.
*
* For pairs where the cross color isn't facing up, this means
* setting them up for a three move insert. Otherwise, ensure
* the corner and edge piece aren't adjacent.
*/
void prepF2LToInsert(Cube& cube, LOCATION cornerLoc, LOCATION edgeLoc, std::vector<Move>& solution)
{
	// do nothing if paired
	if (isF2LPairPaired(cube, cornerLoc, edgeLoc))
		return;

	// remember F2L pair color
	COLOR color0 = cube.getSticker(edgeLoc);
	COLOR color1 = cube.getSticker(cube.getAdjacentEdge(edgeLoc));

	// split up pair if they're adjacent
	splitF2LPair(cube, cornerLoc, edgeLoc, solution);

	// must relocate pair now
	std::pair<LOCATION, LOCATION> pairLoc = locateF2LPair(cube, color0, color1);
	cornerLoc = pairLoc.first;
	edgeLoc = pairLoc.second;

	// if cross color facing to the side (do nothing if facing up; splitting is enough)
	if (cornerLoc.face != FACE::UP)
	{
		// get the stickers facing up on the corner and edge
		COLOR cornerUpColor = cube.getSticker({ FACE::UP, getUpIdxCorner(cornerLoc) });
		COLOR edgeUpColor = edgeLoc.face == FACE::UP ?
			cube.getSticker(edgeLoc) :
			cube.getSticker(cube.getAdjacentEdge(edgeLoc));

		// if they match
		if (cornerUpColor == edgeUpColor)
		{
			// pair up the pieces
			// if cross color is facing left
			if (cornerLoc.idx == 2)
			{
				// need slot at cornerUpIdx + 2 to be unsolved
				moveToUnsolvedSlot(cube, (getUpIdxCorner(cornerLoc) + 2) % 8, solution);

				// relocate F2L pair
				pairLoc = locateF2LPair(cube, color0, color1);
				cornerLoc = pairLoc.first;
				edgeLoc = pairLoc.second;

				// get the target idx for the edge
				uint8_t targetIdx = (getUpIdxCorner(cornerLoc) + 7) % 8;
				uint8_t edgeUpIdx = edgeLoc.face == FACE::UP ? edgeLoc.idx : cube.getAdjacentEdge(edgeLoc).idx;

				// pair up the pieces
				solution.push_back(cube.move(cornerLoc.face));

				// note: only a U and a U2 are possible since pieces are split
				if ((edgeUpIdx + 2) % 8 == targetIdx)
					solution.push_back(cube.move(FACE::UP));
				else if ((edgeUpIdx + 4) % 8 == targetIdx)
					solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));

				solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));
			}
			// if cross color is facing right
			else if (cornerLoc.idx == 0)
			{
				// need slot at cornerUpIdx - 2 to be unsolved
				moveToUnsolvedSlot(cube, (getUpIdxCorner(cornerLoc) + 6) % 8, solution);

				// relocate F2L pair
				pairLoc = locateF2LPair(cube, color0, color1);
				cornerLoc = pairLoc.first;
				edgeLoc = pairLoc.second;

				// get the target idx for the edge
				uint8_t targetIdx = (getUpIdxCorner(cornerLoc) + 1) % 8;
				uint8_t edgeUpIdx = edgeLoc.face == FACE::UP ? edgeLoc.idx : cube.getAdjacentEdge(edgeLoc).idx;

				// pair up the pieces
				solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));

				// note: only a U' and a U2 are possible since pieces are split
				if ((edgeUpIdx + 6) % 8 == targetIdx)
					solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
				else if ((edgeUpIdx + 4) % 8 == targetIdx)
					solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));

				solution.push_back(cube.move(cornerLoc.face));
			}
		}
		// if they don't match
		else
		{
			// first check that they're not already set up
			uint8_t cornerUpIdx = getUpIdxCorner(cornerLoc);
			uint8_t edgeUpIdx = edgeLoc.face == FACE::UP ? edgeLoc.idx : cube.getAdjacentEdge(edgeLoc).idx;

			// if cross color is facing left and they need to be set up
			if (cornerLoc.idx == 2 && (cornerUpIdx + 3) % 8 != edgeUpIdx)
			{
				// they're guaranteed to be split, so only a U' is necessary
				// need slot at cornerUpIdx + 2 to be unsolved
				moveToUnsolvedSlot(cube, (getUpIdxCorner(cornerLoc) + 2) % 8, solution);

				// relocate F2L pair
				pairLoc = locateF2LPair(cube, color0, color1);
				cornerLoc = pairLoc.first;

				// set up the pieces
				solution.push_back(cube.move(cornerLoc.face));
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
				solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));
			}
			// if cross color is facing right and they need to be set up
			else if (cornerLoc.idx == 0 && (edgeUpIdx + 3) % 8 != cornerUpIdx)
			{
				// they're guaranteed to be split, so only a U is necessary
				// need slot at cornerUpIdx + 2 to be unsolved
				moveToUnsolvedSlot(cube, (getUpIdxCorner(cornerLoc) + 2) % 8, solution);

				// relocate F2L pair
				pairLoc = locateF2LPair(cube, color0, color1);
				cornerLoc = pairLoc.first;

				// set up the pieces
				FACE face = cube.getAdjacentFace(cornerLoc.face, "y");
				solution.push_back(cube.move(face));
				solution.push_back(cube.move(FACE::UP));
				solution.push_back(cube.move(face, Move::TYPE::PRIME));
			}
		}
	}
}

/**
* Insert the given F2L pair into its
* correct slot.
*/
void insertF2LPair(Cube& cube, LOCATION cornerLoc, LOCATION edgeLoc, std::vector<Move>& solution)
{
	// get F2L pair colors
	COLOR color0 = cube.getSticker(edgeLoc);
	COLOR color1 = cube.getSticker(cube.getAdjacentEdge(edgeLoc));

	// if pieces are already paired up
	if (isF2LPairPaired(cube, cornerLoc, edgeLoc))
	{
		// if cross color facing left
		if (cornerLoc.idx == 2)
		{
			// need cornerUpIdx - 2 to be positioned over the slot
			moveToSlot(cube, (getUpIdxCorner(cornerLoc) + 6) % 8, color0, color1, solution);

			// relocate the F2L pair
			std::pair<LOCATION, LOCATION> pairLoc = locateF2LPair(cube, color0, color1);

			// insert the pair
			FACE face = cube.getOppositeFace(pairLoc.first.face);
			solution.push_back(cube.move(face));
			solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
			solution.push_back(cube.move(face, Move::TYPE::PRIME));
		}
		// if cross color facing right
		else if (cornerLoc.idx == 0)
		{
			// need cornerUpIdx + 2 to be positioned over the slot
			moveToSlot(cube, (getUpIdxCorner(cornerLoc) + 2) % 8, color0, color1, solution);

			// relocate the F2L pair
			std::pair<LOCATION, LOCATION> pairLoc = locateF2LPair(cube, color0, color1);

			// insert the pair
			FACE face = cube.getOppositeFace(pairLoc.first.face);
			solution.push_back(cube.move(face, Move::TYPE::PRIME));
			solution.push_back(cube.move(FACE::UP));
			solution.push_back(cube.move(face));
		}
	}
	// if cross color is facing up
	else if (cornerLoc.face == FACE::UP)
	{
		// align edge with center
		COLOR toMatch = edgeLoc.face != FACE::UP ? cube.getSticker(edgeLoc) : cube.getSticker(cube.getAdjacentEdge(edgeLoc));
		FACE currFace = edgeLoc.face != FACE::UP ? edgeLoc.face : cube.getAdjacentEdge(edgeLoc).face;

		uint8_t moves = 0;
		while (toMatch != cube.getCenter(currFace))
		{
			cube.u();
			currFace = cube.getAdjacentFace(currFace, "y");
			moves++;
		}

		// update new positions
		edgeLoc.idx = 1;
		edgeLoc.face = currFace;
		cornerLoc.idx = (cornerLoc.idx + (moves * 2)) % 8;

		if (moves == 1)
			solution.push_back(Move(Move::PIECES::UP, Move::TYPE::NORMAL));
		else if (moves == 2)
			solution.push_back(Move(Move::PIECES::UP, Move::TYPE::DOUBLE));
		else if (moves == 3)
			solution.push_back(Move(Move::PIECES::UP, Move::TYPE::PRIME));

		// get the target idx
		uint8_t targetIdx = findF2LSlot(cube, color0, color1);
		uint8_t edgeUpIdx = cube.getAdjacentEdge(edgeLoc).idx;
		uint8_t cornerUpIdx = getUpIdxCorner(cornerLoc);

		// if target is to the right of edge
		if ((targetIdx + 1) % 8 == edgeUpIdx)
		{
			// insert pair
			solution.push_back(cube.move(edgeLoc.face, Move::TYPE::PRIME));

			if ((cornerUpIdx + 3) % 8 == edgeUpIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));
			else if ((cornerUpIdx + 5) % 8 == edgeUpIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));

			solution.push_back(cube.move(edgeLoc.face));
			solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
			solution.push_back(cube.move(edgeLoc.face, Move::TYPE::PRIME));
			solution.push_back(cube.move(FACE::UP));
			solution.push_back(cube.move(edgeLoc.face));
		}
		// if target is to the left of edge
		else if ((edgeUpIdx + 1) % 8 == targetIdx)
		{
			// insert pair
			solution.push_back(cube.move(edgeLoc.face));

			if ((cornerUpIdx + 3) % 8 == edgeUpIdx)
				solution.push_back(cube.move(FACE::UP));
			else if ((cornerUpIdx + 5) % 8 == edgeUpIdx)
				solution.push_back(cube.move(FACE::UP, Move::TYPE::DOUBLE));

			solution.push_back(cube.move(edgeLoc.face, Move::TYPE::PRIME));
			solution.push_back(cube.move(FACE::UP));
			solution.push_back(cube.move(edgeLoc.face));
			solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
			solution.push_back(cube.move(edgeLoc.face, Move::TYPE::PRIME));
		}
	}
	// if cross color is facing to the side
	else
	{
		// position corner over the slot
		moveToSlot(cube, getUpIdxCorner(cornerLoc), color0, color1, solution);

		// relocate the F2L pair
		std::pair<LOCATION, LOCATION> pairLoc = locateF2LPair(cube, color0, color1);
		cornerLoc = pairLoc.first;

		// insert pair
		// cross color facing left
		if (cornerLoc.idx == 2)
		{
			solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));
			solution.push_back(cube.move(FACE::UP, Move::TYPE::PRIME));
			solution.push_back(cube.move(cornerLoc.face));
		}
		// cross color facing right
		else if (cornerLoc.idx == 0)
		{
			solution.push_back(cube.move(cornerLoc.face));
			solution.push_back(cube.move(FACE::UP));
			solution.push_back(cube.move(cornerLoc.face, Move::TYPE::PRIME));
		}
	}
}

/**
* Solve the given F2L pair without disturbing any
* other potentially solved pairs.
*/
void solveF2LPair(Cube& cube, std::pair<LOCATION, LOCATION> pairLoc, std::vector<Move>& solution)
{
	// remember F2L pair's colors (so it can be relocated later)
	COLOR color0 = cube.getSticker(pairLoc.second);
	COLOR color1 = cube.getSticker(cube.getAdjacentEdge(pairLoc.second));

	// bring F2L pair to top
	bringF2LToTop(cube, pairLoc.first, pairLoc.second, solution);
	pairLoc = locateF2LPair(cube, color0, color1);

	// make pair ready for insertion
	prepF2LToInsert(cube, pairLoc.first, pairLoc.second, solution);
	pairLoc = locateF2LPair(cube, color0, color1);

	// insert the pair
	insertF2LPair(cube, pairLoc.first, pairLoc.second, solution);
}

/**
* Solve the first two layers on the given cube.
*
* Assumes the cross is solved and is oriented down.
*/
void solveF2L(Cube& cube, std::vector<Move>& solution)
{
	// get the cross color
	COLOR color = cube.getCenter(FACE::DOWN);

	std::pair<std::pair<LOCATION, LOCATION>, bool> pairLoc = findUnsolvedF2LPair(cube, color);
	while (pairLoc.second)
	{
		solveF2LPair(cube, pairLoc.first, solution);
		solution.push_back(Move(Move::PIECES::Y, Move::TYPE::NO_MOVE));
		pairLoc = findUnsolvedF2LPair(cube, color);
	}
}
