
#pragma once

#include <string>
#include <optional>
 
class COptions {
    public: 
        COptions(const std::string& name, int argc, char** argv);
        std::filesystem::path mPath;
        int mLimit = 10;

    private:
        bool parse(const std::string& name, int argc, char** argv);
};