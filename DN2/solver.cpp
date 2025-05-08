
#include "solver.h"
#include "pch.h"
#include <fmt/core.h>

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
    if ( !std::filesystem::exists( path ) ) 
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
    std::vector<std::vector<int>> memo( n, std::vector<int>( k, 0 ) );
    std::function<int( int, int )> solver = [&]( int i, int j ) -> int {
        if( i < 0 || j < 0 || i > n || j > k )
            return std::numeric_limits<int>::min();

        if( i == 0 || j == 0 )
            return 0;

        auto& mem = memo.at( i - 1 ).at( j - 1 );
        if( mem > 0 ) 
            return mem;

        const auto& number = numbers.at(i - 1);
        mem = std::max( solver( i - 1, j ), solver( i - 1, j - number ) + number );
        return mem;
    };
    auto solution = solver( n, k );
    std::cout << fmt::format( "Solution found was {}.\n", solution );
    return solution == k;
}

bool CSolver::solveEXH( int n, int k, const std::vector<int>& numbers )
{
    auto sortedNumbers = numbers;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());

    std::set<int> sums = {0};
    for ( int i = 1; i < n; i++ ) {
        auto currSums = sums;

        for ( const auto element : currSums ) {
            auto newElement = element + sortedNumbers.at(i);
            if ( newElement <= k )
                sums.insert(newElement);
        }
    }
    // NOTE: A set is sorted so the last element is the max element.
    auto solution = *sums.rbegin();
    std::cout << fmt::format( "Maximal element in the final set was {}.\n", solution );
    return solution == k;
}       

bool CSolver::solveGRDY( int n, int k, const std::vector<int>& numbers )
{
    int target = 0;
    for ( int i = 0; i < n; i++ ) {
        const auto& element = numbers.at( i );
        if (k - target >= element )
            target += element;
    }
    auto approx = fmt::format( "The approximation for {} is {}.\n", k, target );
    approx += fmt::format( "Difference is {}.\n", k - target );
    std::cout << approx;
    return target == k;
}  

bool CSolver::solveFPTAS( int n, int k, const std::vector<int>& numbers )
{
    return {};
}  