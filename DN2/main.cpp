#include "pch.h"
#include "options.h"
#include "solver.h"
#include <fmt/core.h>

int main( int argc, char** argv ) {
    COptions options( "DYN Solver", argc, argv );
    CSolver solver;
    auto oSolution = solver.solve( options.mPath, EMode(options.mMode) );
    if( !oSolution ) {
        std::cout << "Solution does not exit.";
        return 0;
    }
    std::string sol = "Solution: ";
    for(int number : *oSolution)
        sol += fmt::format("{} ", number);
    std::cout << sol;
}