
#pragma once

#include <string>
 
class COptions {
    public: 
        COptions(const std::string& name, int argc, char** argv);
        std::filesystem::path mPath;
        int mMode;

    private:
        bool parse(const std::string& name, int argc, char** argv);
};