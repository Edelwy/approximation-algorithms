#pragma once

#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <boost/graph/adjacency_list.hpp>

// NOTE: Edges have weights in order to use the BOOST library
// implementation for finding the optimum solution, our algorithm
// doesn't need it. Weights are being set to 1.

typedef boost::adjacency_list< 
        boost::vecS, // Vector vertex container.
        boost::vecS, // Vector edge container.
        boost::undirectedS, // Undirected graph.
        boost::no_property,  // Vertices have no properties.                    
        boost::property<boost::edge_weight_t, int>> // Edges have weights.
        graph_t;

typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;
typedef boost::graph_traits<graph_t>::edge_descriptor edge_t;

// NOTE: Result is created after calling the solve function.
// This is a common way of storing results inside the solver itself.

struct CResult {
    std::string mName;
    int mSizeV;
    int mSizeE;
    int mIterations;
    int mSolution;
    int mOptimum;
};

class CSolver {
        std::optional<CResult> mResult;

    public:
        CSolver() = default;

        std::optional<CResult> result();

        bool solve( const std::filesystem::path& path, int maxIterations );
    
    private:
        graph_t parse( const std::filesystem::path& path );

        int optimum( const std::filesystem::path& path );
};   