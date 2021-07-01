#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <cmath>

/**
* Right shift source by the specified number of bits
* with wrap around.
*/
uint64_t rotateRight(uint64_t source, uint64_t bits);

/**
* Left shift source by the specified number of bits
* with wrap around.
*/
uint64_t rotateLeft(uint64_t source, uint64_t bits);

#endif
