
#include "options.h"
#include "pch.h"
#include <CLI/CLI.hpp>

COptions::COptions(const std::string& name, int argc, char** argv) {
    parse(name, argc, argv);
}

bool COptions::parse(const std::string& name, int argc, char** argv) {

    CLI::App app{name};
    app.add_option("--path", mPath, "The path to the input file.")->required();
    app.add_option("--mode", mMode, "The algorithm implementation.");
    app.add_flag("--gen", mGenerate, "Generate the test cases.");
    
    double epsilon;
    auto pOption = app.add_option("--eps", epsilon, "Epsilon for the FPTAS approximation.");
        
    CLI11_PARSE(app, argc, argv);
    if( *pOption )
        moEpsilon = epsilon;

    return true;
}