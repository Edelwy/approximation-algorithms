
#include "pch.h"
#include "benchmark.h"
#include "path.h"
#include  <fmt/core.h>

std::map<EMode, std::string> CBenchmark::modeMap {
    { EMode::DYN, "DYN" },
    { EMode::EXH, "EXH" },
    { EMode::GRDY, "GRDY" },
    { EMode::FPTAS, "FPTAS" }
};

CBenchmark::CBenchmark(double epsilon)
    : mSolver(epsilon){}

void CBenchmark::setEpsilon( double epsilon )
{
    mSolver.setEpsilon( epsilon );
}

void CBenchmark::start( const std::filesystem::path& path )
{
    for ( const auto& path : CPath::paths( path ) ) {
        auto oName = CPath::name( path );
        if ( !CPath::isFile( path ) || !oName )
            continue;

        mSolver.solve( path, EMode::DYN );
        std::cout << report( *oName, EMode::DYN );

        mSolver.solve( path, EMode::EXH );
        std::cout << report( *oName, EMode::EXH );

        mSolver.solve( path, EMode::GRDY );
        std::cout << report( *oName, EMode::GRDY );

        mSolver.solve( path, EMode::FPTAS );
        std::cout << report( *oName, EMode::FPTAS );
    }
}

bool CBenchmark::generate( const std::filesystem::path& path )
{
    if ( !generateDYN( path ) )
        return false;
    if ( !generateEXH( path ) )
        return false;
    if ( !generateGRDY( path ) )
        return false;
    if ( !generateFPTAS( path ) )
        return false;
    return true;
}

bool CBenchmark::clean( const std::filesystem::path& path )
{   
    return CPath::clean( path );
}

bool CBenchmark::generateDYN( const std::filesystem::path& path )
{
    return false;
}

bool CBenchmark::generateEXH( const std::filesystem::path& path )
{
    return false;
}        

bool CBenchmark::generateGRDY( const std::filesystem::path& path )
{
    return false;
}   

bool CBenchmark::generateFPTAS( const std::filesystem::path& path )
{
    return false;
} 

std::string CBenchmark::report( const std::string& name, EMode mode) 
{
    auto oResult = mSolver.result();
    if( !oResult )
        return "No result obtained from solver.";

    std::string reported = fmt::format( "File {:>4}  |  ", name );
    reported += fmt::format( "Mode {:>5}  |  ", modeMap[ mode ] );
    reported += fmt::format( "Eps {:>2}  |  ", mSolver.getEpsilon() );
    reported += fmt::format( "N {:>6}  |  ", oResult->listSize );
    reported += fmt::format( "K {:>8}  |  ", oResult->sumLimit );
    reported += fmt::format( "Maximum {:>8}  |  ", oResult->maxSum);
    reported += fmt::format( "Diff {:>5}  |  ", oResult->difference);
    reported += fmt::format( "Time {:>5.5f}\n",  oResult->duration );
    return reported;
}