
#pragma once

#include <string>
#include <optional>
 
class COptions {
    public: 
        COptions(const std::string& name, int argc, char** argv);
        std::filesystem::path mPath;
        std::optional<double> moEpsilon;
        int mMode;

    private:
        bool parse(const std::string& name, int argc, char** argv);
};