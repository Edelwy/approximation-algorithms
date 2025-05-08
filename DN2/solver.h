
#pragma once

#include <vector>
#include <string>

enum class EMode {
    DYN =   1,
    EXH =   2,
    GRDY =  3,
    FPTAS = 4,
};

class CSolver {
        double mEpsilon = 0.05;
    public:
        CSolver() = default;
        CSolver(double epsilon);

        void setEpsilon( double epsilon );

        bool solve( const std::filesystem::path& path, EMode mode );
    
    private:
        bool parse( const std::filesystem::path& path, int& n, int& k, std::vector<int>& numbers );

        bool solveDYN( int n, int k, const std::vector<int>& numbers );

        bool solveEXH( int n, int k, const std::vector<int>& numbers );        

        bool solveGRDY( int n, int k, const std::vector<int>& numbers );   

        bool solveFPTAS( int n, int k, const std::vector<int>& numbers );   
};   