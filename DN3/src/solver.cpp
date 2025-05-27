
#include "solver.h"
#include "path.h"
#include "pch.h"
#include "timer.h"
#include <fmt/core.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/pending/disjoint_sets.hpp>

std::optional<CResult> CSolver::result()
{
    return mResult;
}

bool CSolver::solve( const std::filesystem::path& path, int maxIterations )
{
    // Initialize the problem.
    auto graph = parse( path );

    // Obtain edges as a vector.
    std::vector<edge_t> edges;
    for( const auto& edge : boost::make_iterator_range( boost::edges( graph ) ) )
        edges.push_back( edge );

    int sizeV = boost::num_vertices( graph );
    int sizeE = edges.size();

    // Random number generator.
    std::random_device rd;
    std::mt19937 gen( rd() );

    // Contract the graph until maximum number of iterations is used
    // or an optimal solution is found.
    auto minimum = sizeE;
    int iteration = 1;
    while( iteration <= maxIterations ) {
        std::vector<int> rank( sizeV );
        std::vector<int> parent( sizeV );

        boost::disjoint_sets<int*, int*> ds(&rank[0], &parent[0]);
        for (int v = 0; v < sizeV; ++v)
            ds.make_set(v);

        std::vector<edge_t> shuffledEdges = edges;
        std::shuffle( shuffledEdges.begin(), shuffledEdges.end(), gen );

        int components = sizeV;
        for( const auto& e : shuffledEdges ) {
            int u = boost::source( e, graph );
            int v = boost::target( e, graph );

            if( ds.find_set( u ) != ds.find_set( v ) ) {
                if( components <= 2 ) 
                    break;
                ds.union_set( u, v );
                components--;
            }
        }

        int cutCount = 0;
        for( const auto& e : shuffledEdges ) {
            int u = source( e, graph );
            int v = target( e, graph );
            if( ds.find_set( u ) != ds.find_set( v ) ) 
                cutCount++;
        }

        minimum = std::min(minimum, cutCount);
        iteration++;
    }

    // Set these as parameters of the result.
    CResult result;
    auto oName = CPath::name( path );
    if( !oName )
        return false;
    result.mName = *oName;
    result.mSizeV = sizeV;
    result.mSizeE = sizeE;
    result.mMinimum = minimum;
    result.mIterations = iteration - 1;
    mResult = result;
    return true;
}

graph_t CSolver::parse( const std::filesystem::path& path )
{
    if ( !CPath::exists( path ) ) 
        return false;

    std::ifstream infile( path );
    if ( !infile ) 
        return false;

    int v1; 
    int v2;
    graph_t graph;
    while ( infile >> v1 >> v2 ) 
        boost::add_edge( v1, v2, graph );
        
    return graph;
}