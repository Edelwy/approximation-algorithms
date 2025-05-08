#include "pch.h"
#include "options.h"
#include "solver.h"
#include <fmt/core.h>

int main( int argc, char** argv ) {
    COptions options( "DYN Solver", argc, argv );
    CSolver solver;
    auto solution = solver.solve( options.mPath, EMode(options.mMode) );
    solution ? std::cout << "Solution exists.\n" : std::cout << "Solution does not exist.\n";
}