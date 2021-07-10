/**
 * Module to bind with embind, following this tutorial
 * https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
*/
#include <iostream>
#include <string>
#include <emscripten/bind.h>

#include "Solver.h"

using namespace emscripten;

Cube c;
std::vector<Move> solution;

/**
 * Function to compile to web assembly for use by web-based Rubik's Cube.
 * 
 * Takes in a string representation of a cube and returns a move sequence
 * that solves that cube state.
*/
std::string getSolution(std::string state)
{
    c.copyState(state);
    solution = solve(c);
    solution = cleanSolution(solution, true);
    return solutionToString(solution);
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("getSolution", &getSolution);
}
