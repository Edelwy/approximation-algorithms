#pragma once

#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;
typedef boost::graph_traits<graph_t>::edge_descriptor edge_t;

struct CResult {
    std::string mName;
    int mSizeV;
    int mSizeE;
    int mIterations;
    int mMinimum;
};

class CSolver {
        std::optional<CResult> mResult;

    public:
        CSolver() = default;

        std::optional<CResult> result();

        bool solve( const std::filesystem::path& path, int maxIterations );
    
    private:
        graph_t parse( const std::filesystem::path& path );
};   