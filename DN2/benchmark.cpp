
#include "pch.h"
#include "benchmark.h"
#include "path.h"
#include  <fmt/core.h>

namespace {

int getRand(int from, int to) 
{
    std::mt19937 randomGen( std::random_device{}() );
    std::uniform_int_distribution<int> dist(from, to);
    return dist(randomGen);
}   

}

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

void CBenchmark::start( const std::filesystem::path& path, EMode mode )
{
    if ( CPath::isFile( path ) ) {
        auto oName = CPath::name( path );
        if ( !oName )
            return;
        mSolver.solve( path, mode );
        std::cout << report( *oName, mode );
    }
    else if ( CPath::isDirectory( path ) ) {
        for ( const auto& path : CPath::paths( path ) ) {
            auto oName = CPath::name( path );
            if ( !oName || !CPath::isFile( path ) )
                continue;
            mSolver.solve( path, mode );
            std::cout << report( *oName, mode );
        }
    }
}

void CBenchmark::startEpsilon( const std::filesystem::path& path ) 
{
    for ( double eps = 0.1; eps < 3.0001; eps += 0.1 ) {
        mSolver.setEpsilon( eps );
        start( path, EMode::FPTAS );
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
    int n = 5000;
    std::vector<int> ones( n, 1 );
    write( path, "1", n, 4000, ones );

    int k = 0;
    std::vector<int> numbers;
    for ( int i = 0; i < n; i++ ) {
        auto random = getRand(1, 40);
        k += random;
        numbers.push_back(random);
    }
    write( path, "2", n, k + 1, numbers );
    return true;
}

bool CBenchmark::generateEXH( const std::filesystem::path& path )
{
    int k = 0;
    int n = 25;
    std::vector<int> numbers;
    for ( int i = 0; i < n; i++ ) {
        auto power = std::pow(2, i);
        k += power;
        numbers.push_back(power);
    }
    write( path, "3", n, k + 1, numbers );
    return true;
}        

bool CBenchmark::generateGRDY( const std::filesystem::path& path  )
{
    std::vector<int> numbers = { 1, 2000 };
    write( path, "4", 2, 2000, numbers );

    numbers = { 51, 50, 50 };
    write( path, "5", 3, 100, numbers );

    return true;
}   

bool CBenchmark::generateFPTAS( const std::filesystem::path& path )
{
    int k = 100;
    int n = 20;
    std::vector<int> numbers( n, 1 );
    numbers.push_back( k );
    write( path, "6", n + 1, k, numbers );
    return true;
} 

std::string CBenchmark::report( const std::string& name, EMode mode) 
{
    auto oResult = mSolver.result();
    if( !oResult )
        return "No result obtained from solver.";

    std::string reported = fmt::format( "File {:>4}  |  ", name );
    reported += fmt::format( "Mode {:>5}  |  ", modeMap[ mode ] );
    reported += fmt::format( "Eps {:>2.2f}  |  ", mSolver.getEpsilon() );
    reported += fmt::format( "N {:>6}  |  ", oResult->listSize );
    reported += fmt::format( "K {:>8}  |  ", oResult->sumLimit );
    reported += fmt::format( "Maximum {:>8}  |  ", oResult->maxSum);
    reported += fmt::format( "Diff {:>5}  |  ", oResult->difference);
    reported += fmt::format( "Time {:>5.5f}\n",  oResult->duration );
    return reported;
}

bool CBenchmark::write( const std::filesystem::path& path, 
    const std::string& name, int n, int k, std::vector<int> numbers) 
{
    std::filesystem::path filePath = path;
    if ( CPath::isDirectory( path ) )
        filePath = path / fmt::format( "{}.txt", name );
    std::ofstream fout( filePath );
    if( !fout ) 
        return false;

    fout << n << ' ' << k << "\n";
    for (int num : numbers) 
        fout << num << "\n";

    fout.close();
    return true;
}