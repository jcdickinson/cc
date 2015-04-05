#include "stdafx.h"
#include "contentmanager.h"

using namespace std;
using namespace std::tr2::sys;

namespace content {

  LoadOperation::LoadOperation(content::ContentManager& contentManager, const string path, const string fullPath, const std::shared_ptr<std::istream> data) :
    ContentManager(contentManager),
    Path(path),
    FullPath(fullPath),
    Data(data) {

  }

  LoadOperation::~LoadOperation( ) {

  }

  static const std::tr2::sys::path ResolveName(const std::string path, bool includesExeName) {
    auto originalPath = std::tr2::sys::path(path);
    auto finalPath = std::tr2::sys::path( );
    for (auto it = originalPath.begin( ); it != originalPath.end( ); ++it) {
      if (*it == "..") {
        if (std::tr2::sys::is_symlink(finalPath))
          finalPath /= *it;
        else if (finalPath.filename( ) == "..")
          finalPath /= *it;
        else
          finalPath = finalPath.parent_path( );
      } else if (*it == ".") {

      } else {
        finalPath /= *it;
      }
    }
    if (includesExeName)
      finalPath.remove_filename( );
    return finalPath;
  }

  ContentManager::ContentManager(const string basePath, bool includesExeName) :
    _basePath(ResolveName(basePath, includesExeName)) {

  }

  ContentManager::~ContentManager( ) {

  }

}