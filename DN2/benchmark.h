
#pragma once

#include <map>
#include <string>
#include <filesystem>
#include "solver.h"

class CBenchmark {
        CSolver mSolver;
        static std::map<EMode, std::string> modeMap;

    public:
        CBenchmark() = default;
        CBenchmark( double epsilon );

        void setEpsilon( double epsilon );

        void start( const std::filesystem::path& path );

        void start( const std::filesystem::path& path, EMode mode );

        bool generate( const std::filesystem::path& path, int n );

        bool clean( const std::filesystem::path& path );
    
    private:
        bool generateDYN( const std::filesystem::path& path, int n );

        bool generateEXH( const std::filesystem::path& path, int n );        

        bool generateGRDY( const std::filesystem::path& path, int n );   

        bool generateFPTAS( const std::filesystem::path& path, int n );
        
        std::string report( const std::string& name, EMode mode );

        bool write( const std::filesystem::path& path, 
            const std::string& name, int n, int k, std::vector<int> numbers);
};   