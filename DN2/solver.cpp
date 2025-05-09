
#include "solver.h"
#include "path.h"
#include "pch.h"
#include "timer.h"
#include <fmt/core.h>

CSolver::CSolver( double epsilon ) 
    : mEpsilon( epsilon ){};

void CSolver::setEpsilon( double epsilon ) 
{
    mEpsilon = epsilon;
}

double CSolver::getEpsilon() 
{
    return mEpsilon;
}

std::optional<CResult> CSolver::result()
{
    return mResult;
}

bool CSolver::solve( const std::filesystem::path& path, EMode mode )
{
    int n, k;
    std::vector<int> numbers = {};
    parse( path, n, k, numbers );
    
    switch ( mode ) {
    case EMode::DYN:
        return solveDYN( n, k, numbers );
    case EMode::EXH:
        return solveEXH( n, k, numbers );
    case EMode::GRDY:
        return solveGRDY( n, k, numbers );
    case EMode::FPTAS:
        return solveFPTAS( n, k, numbers );
    default:
        return false;
    }
}

bool CSolver::parse( const std::filesystem::path& path, int& n, int& k, std::vector<int>& numbers )
{
    if ( !CPath::exists( path ) ) 
        return false;

    std::ifstream infile( path );
    if ( !infile ) 
        return false;

    infile >> n >> k;
    int number;
    while ( infile >> number ) 
        numbers.push_back( number );
    return true;
}

bool CSolver::solveDYN( int n, int k, const std::vector<int>& numbers )
{
    CTimer timer;
    timer.start();

    std::vector<std::vector<int>> memo( n, std::vector<int>( k, 0 ) );
    std::function<int( int, int )> solver = [&]( int i, int j ) -> int {
        if( i < 0 || j < 0 || i > n || j > k )
            return std::numeric_limits<int>::min();

        if( i == 0 || j == 0 )
            return 0;

        auto& mem = memo.at( i - 1 ).at( j - 1 );
        if( mem > 0 ) 
            return mem;

        const auto& num = numbers.at( i - 1 );
        mem = std::max( solver( i - 1, j ), solver( i - 1, j - num ) + num );
        return mem;
    };

    CResult result;
    result.duration = timer.stop();
    result.maxSum = solver( n, k );
    result.listSize = n;
    result.sumLimit = k;
    result.solvable = result.maxSum  == k;
    result.difference = k - result.maxSum;

    mResult = result;
    return result.solvable;
}

bool CSolver::solveEXH( int n, int k, const std::vector<int>& numbers )
{
    CTimer timer;
    timer.start();

    std::set<int> sums = { 0 };
    for ( int i = 1; i < n; i++ ) {
        auto currSums = sums;

        for ( const auto element : currSums ) {
            auto newElement = element + numbers.at( i );
            if ( newElement <= k )
                sums.insert( newElement );
        }
    }

    CResult result;
    result.duration = timer.stop();
    result.maxSum = *sums.rbegin();
    result.listSize = n;
    result.sumLimit = k;
    result.solvable = result.maxSum  == k;
    result.difference = k - result.maxSum;

    mResult = result;
    return result.solvable;
}       

bool CSolver::solveGRDY( int n, int k, const std::vector<int>& numbers )
{
    CTimer timer;
    timer.start();

    auto sortedNumbers = numbers;
    std::sort( sortedNumbers.begin(), sortedNumbers.end(), std::greater<int>() );

    int solution = 0;
    for ( int i = 0; i < n; i++ ) {
        const auto& element = sortedNumbers.at( i );
        if ( k - solution >= element )
            solution += element;
    }

    CResult result;
    result.duration = timer.stop();
    result.maxSum = solution;
    result.listSize = n;
    result.sumLimit = k;
    result.solvable = result.maxSum  == k;
    result.difference = k - result.maxSum;

    mResult = result;
    return result.solvable;
}  

bool CSolver::solveFPTAS( int n, int k, const std::vector<int>& numbers )
{
    CTimer timer;
    timer.start();

    auto delta = mEpsilon / ( 2 * n );
    std::set<int> sums = { 0 };
    for ( int i = 1; i < n; i++ ) {
        auto tmpSums = sums;

        for ( const auto element : tmpSums ) 
            sums.insert( element + numbers.at( i ) );

        auto last = *sums.begin();
        tmpSums = { last };
        for ( auto& element : sums ) {
            if( element <= k && element > last * ( 1 + delta ) ) {
                tmpSums.insert( element );
                last = element;
            }
        }
        sums = tmpSums;
    }

    CResult result;
    result.duration = timer.stop();
    result.maxSum = *sums.rbegin();
    result.listSize = n;
    result.sumLimit = k;
    result.solvable = result.maxSum  == k;
    result.difference = k - result.maxSum;

    mResult = result;
    return result.solvable;
}  