# Date: 2025/05/27
# Author: Nina Mislej

# CMake version requirements.
cmake_minimum_required(VERSION 3.17)
message("CMake version 3.17 required.")

# CPP version requirements.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message("C++ version 20 required.")

# Set to release when finished.
set(CMAKE_BUILD_TYPE Debug)

# Project initialization.
project(MinCutSolver)

# Fetch external libraries from GitHub.
include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        master  
)

message("Fetching CLI11 library from GitHub.")
FetchContent_MakeAvailable(CLI11)
message("CLI11 library successfully fetched.")

message("Fetching FMT library from GitHub.")
FetchContent_MakeAvailable(fmt)
message("FMT library successfully fetched.")

message("Finding BOOST library installation.")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
message("BOOST library headers available.")

# Adding executables and adding the external libraries.
message("Adding Min Cut Solver executable.")
add_executable(solver.out 
    solverCli.cpp 
    options.cpp
    solver.cpp
    path.cpp)
target_link_libraries(solver.out PRIVATE CLI11::CLI11)
target_link_libraries(solver.out PRIVATE fmt::fmt)
target_precompile_headers(solver.out PRIVATE pch.h)