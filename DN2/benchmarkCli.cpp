#include "pch.h"
#include "options.h"
#include "benchmark.h"
#include <fmt/core.h>

int main( int argc, char** argv ) 
{
    COptions options( "Benchmark", argc, argv );
    CBenchmark benchmark;
    if( options.moEpsilon )
        benchmark.setEpsilon( *options.moEpsilon );
    
    if ( options.mGenerate > 0 )
        benchmark.generate( options.mPath, options.mGenerate );
    benchmark.start( options.mPath );
}