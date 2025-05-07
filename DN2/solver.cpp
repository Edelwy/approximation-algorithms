
#include "solver.h"
#include "pch.h"

std::optional<std::vector<int>> CSolver::solve(const std::filesystem::path& path, EMode mode){
    int n, k;
    std::vector<int> numbers = {};
    parse(path, n, k, numbers);
    
    switch (mode) {
    case EMode::DYN:
        return solveDYN(n, k, numbers);
    case EMode::EXH:
        return solveEXH(n, k, numbers);
    case EMode::GRDY:
        return solveGRDY(n, k, numbers);
    case EMode::FPTAS:
        return solveFPTAS(n, k, numbers);
    default:
        return {};
    }
}

bool CSolver::parse(const std::filesystem::path& path, int& n, int& k, std::vector<int>& numbers){
    if (!std::filesystem::exists(path)) 
        return false;

    std::ifstream infile(path);
    if (!infile) 
        return false;

    infile >> n >> k;
    int number;
    while (infile >> number) 
        numbers.push_back(number);
    return true;
}

std::optional<std::vector<int>> CSolver::solveDYN(int n, int k, const std::vector<int>& numbers){
    return {};
}

std::optional<std::vector<int>> CSolver::solveEXH(int n, int k, const std::vector<int>& numbers){
    return {};
}       

std::optional<std::vector<int>> CSolver::solveGRDY(int n, int k, const std::vector<int>& numbers){
    return {};
}  

std::optional<std::vector<int>> CSolver::solveFPTAS(int n, int k, const std::vector<int>& numbers){
    return {};
}  