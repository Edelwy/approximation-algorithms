
#pragma once

#include <optional>
#include <vector>
#include <string>

enum class EMode {
    DYN =   1,
    EXH =   2,
    GRDY =  3,
    FPTAS = 4,
};

class CSolver {
    public:
        std::optional<std::vector<int>> solve(const std::filesystem::path& path, EMode mode);
    
    private:
        bool parse(const std::filesystem::path& path, int& n, int& k, std::vector<int>& numbers);

        std::optional<std::vector<int>> solveDYN(int n, int k, const std::vector<int>& numbers);

        std::optional<std::vector<int>> solveEXH(int n, int k, const std::vector<int>& numbers);        

        std::optional<std::vector<int>> solveGRDY(int n, int k, const std::vector<int>& numbers);   

        std::optional<std::vector<int>> solveFPTAS(int n, int k, const std::vector<int>& numbers);   
};   