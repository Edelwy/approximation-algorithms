
#pragma once

#include <string>
#include <optional>
 
class COptions {
    public: 
        COptions(const std::string& name, int argc, char** argv);
        std::filesystem::path mPath;
        std::optional<double> moEpsilon;
        int mMode = 1;
        int mGenerate = 0;

    private:
        bool parse(const std::string& name, int argc, char** argv);
};