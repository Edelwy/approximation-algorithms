
#include "solver.h"
#include "path.h"
#include "pch.h"
#include "timer.h"
#include <fmt/core.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/property_map/property_map.hpp>

std::optional<CResult> CSolver::result()
{
    return mResult;
}

bool CSolver::solve( const std::filesystem::path& path, int maxIterations )
{
    // Initialize the problem.
    auto graph = parse( path );
    auto opt = optimum( path );

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
        iteration++;
        std::vector<int> rank( sizeV );
        std::vector<int> parent( sizeV );

        boost::disjoint_sets<int*, int*> ds(&rank[0], &parent[0]);
        for( int v = 0; v < sizeV; v++ )
            ds.make_set( v );

        int components = sizeV;
        std::uniform_int_distribution<> dis(0, edges.size() - 1);
        while( components > 2 ) {
            const edge_t& e = edges[dis(gen)];
            int u = boost::source( e, graph );
            int v = boost::target( e, graph );

            if( ds.find_set( u ) != ds.find_set( v ) ) {
                ds.union_set( u, v );
                components--;
            }
        }

        int cutCount = 0;
        for( const auto& e : edges ) {
            int u = source( e, graph );
            int v = target( e, graph );
            if( ds.find_set( u ) != ds.find_set( v ) ) 
                cutCount++;
        }

        minimum = std::min(minimum, cutCount);
        if( opt == minimum )
            break;
    }

    // Set these as parameters of the result.
    CResult result;
    auto oName = CPath::name( path );
    if( !oName )
        return false;
    result.mName = *oName;
    result.mSizeV = sizeV;
    result.mSizeE = sizeE;
    result.mSolution = minimum;
    result.mIterations = iteration - 1;
    result.mOptimum = opt;
    mResult = result;
    return true;
}

int CSolver::optimum( const std::filesystem::path& path )
{
    auto graph = parse( path );
    int sizeV = boost::num_vertices( graph );
    int sizeE = boost::num_edges( graph );

    std::vector<int> parities( sizeV );
    auto parity_map = boost::make_iterator_property_map( 
        parities.begin(), 
        boost::get( boost::vertex_index, graph ) 
    );

    return stoer_wagner_min_cut(
        graph, 
        get( boost::edge_weight, graph ), 
        boost::parity_map( parity_map ) 
    );

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
        boost::add_edge( v1, v2, 1, graph );
    return graph;
}