#include "pch.h"
#include "options.h"
#include "solver.h"
#include <fmt/core.h>

int main( int argc, char** argv ) {
    COptions options( "Solver", argc, argv );
    CSolver solver;
    auto solution = solver.solve( options.mPath, 30 );

    auto oResult = solver.result();
    if( !solution || !oResult )
        std::cout << "No solution was found.";
    
    std::string reported = fmt::format( "Name {:>7}  |  ", oResult->mName );
    reported += fmt::format( "V {:>6}  |  ", oResult->mSizeV );
    reported += fmt::format( "E {:>6}  |  ", oResult->mSizeE );
    reported += fmt::format( "Solution {:>8}  |  ", oResult->mSolution);
    reported += fmt::format( "Optimum {:>8}  |  ", oResult->mOptimum);
    reported += fmt::format( "Iterations {:>5}\n",  oResult->mIterations );
    std::cout << reported;
}