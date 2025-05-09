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

    if ( options.mbGenerate > 0 )
        benchmark.generate( options.mPath );

    if ( options.mbEpsilon > 0 )
        benchmark.startEpsilon( options.mPath );
    else if ( options.mMode > 0 )
        benchmark.start( options.mPath, EMode( options.mMode) );
    else   
        benchmark.start( options.mPath );
}