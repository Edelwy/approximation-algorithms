
#pragma once

#include <filesystem>
#include <optional>
#include <set>

class CPath
{
public:
   static bool create( const std::filesystem::path& dir );

   static bool remove( const std::filesystem::path& dir );

   static bool exists( const std::filesystem::path& dir );

   static bool clean( const std::filesystem::path& dir );

   static bool isFile( const std::filesystem::path& dir );

   static bool isDirectory( const std::filesystem::path& dir );

   static std::set<std::filesystem::path> paths( const std::filesystem::path& dir );

   static std::optional<std::string> name( const std::filesystem::path& dir );

};
