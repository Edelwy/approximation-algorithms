
#pragma once

#include <string>
#include <optional>
 
class COptions {
    public: 
        COptions(const std::string& name, int argc, char** argv);
        std::filesystem::path mPath;
        std::optional<double> moEpsilon;
        int mMode = 1;

        bool mbGenerate = false;
        bool mbEpsilon = false;

    private:
        bool parse(const std::string& name, int argc, char** argv);
};