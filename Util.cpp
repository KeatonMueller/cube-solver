#include "Util.h"

uint64_t rotateRight(uint64_t source, uint64_t bits)
{
	uint64_t toSave = source & ((uint64_t)pow(2, bits) - 1);
	return (source >> bits) | (toSave << (64 - bits));
}

uint64_t rotateLeft(uint64_t source, uint64_t bits)
{
	uint64_t toSave = source & ((uint64_t)(pow(2, bits) - 1) << (64 - bits));
	return (source << bits) | (toSave >> (64 - bits));
}
