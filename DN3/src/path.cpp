
#include "pch.h"
#include "path.h"

bool CPath::create( const std::filesystem::path& dir )
{
   try {
      return std::filesystem::create_directories( dir );
   }
   catch ( ... ) {
      return false;
   }
}

bool CPath::remove( const std::filesystem::path& dir )
{
   try {
      return std::filesystem::remove_all( dir );
   }
   catch ( ... ) {
      return false;
   }
}

bool CPath::exists( const std::filesystem::path& dir )
{
   try {
      return std::filesystem::exists( dir );
   }
   catch ( ... ) {
      return false;
   }
}

std::set<std::filesystem::path> CPath::paths( const std::filesystem::path& dir )
{
   try {
      std::set<std::filesystem::path> paths;
      for (const auto & entry : std::filesystem::directory_iterator( dir ) )
         paths.insert( entry.path() );
      return paths;
   }
   catch ( ... ) {
      return {};
   }
}

std::optional<std::string> CPath::name( const std::filesystem::path& dir )
{
   try {
      return dir.stem().string();
   }
   catch ( ... ) {
      return {};
   }
}

bool CPath::isFile( const std::filesystem::path& dir )
{
   try {
      return std::filesystem::is_regular_file( dir );
   }
   catch ( ... ) {
      return {};
   }
}

bool CPath::isDirectory( const std::filesystem::path& dir )
{
   try {
      return std::filesystem::is_directory( dir );
   }
   catch ( ... ) {
      return {};
   }
}

bool CPath::clean( const std::filesystem::path& dir )
{
   remove( dir );
   return create( dir );
}
