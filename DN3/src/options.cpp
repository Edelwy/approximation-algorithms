
#include "options.h"
#include "pch.h"
#include <CLI/CLI.hpp>

COptions::COptions(const std::string& name, int argc, char** argv) {
    parse(name, argc, argv);
}

bool COptions::parse(const std::string& name, int argc, char** argv) {

    CLI::App app{name};
    app.add_option("--path", mPath, "The path to the input file.")->required();

    CLI11_PARSE(app, argc, argv);
    return true;
}