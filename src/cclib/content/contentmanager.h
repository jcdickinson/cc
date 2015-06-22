#pragma once
#include <filesystem>
#include <functional>
#include <fstream>
#include <istream>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <typeindex>

#include "../engineexception.h"

namespace std {
  template<> struct hash < std::tuple<std::type_index, std::string> > {
    std::size_t operator()(const std::tuple<std::type_index, std::string>& k) const {
      return
        (std::hash<std::type_index>( )(std::get<0>(k))) ^
        (std::hash<std::string>( )(std::get<1>(k)) >> 1);
    }
  };
}

namespace content {

  class ContentManager;

  struct LoadOperation {
    LoadOperation(const LoadOperation&) = default;
    LoadOperation& operator=(const LoadOperation&) = delete;

    LoadOperation(ContentManager& contentManager, const std::string path, const std::string fullPath, const std::shared_ptr<std::istream> data);
    ~LoadOperation( );

    ContentManager& ContentManager;
    const std::string Path;
    const std::string FullPath;
    const std::shared_ptr<std::istream> Data;
  };

  class ContentManager {
    public:
    ContentManager(const ContentManager&) = default;
    ContentManager& operator=(const ContentManager&) = delete;

    ContentManager(const std::string basePath, bool includesExeName = true);
    ~ContentManager( );

    template <typename T> std::shared_ptr<T> LoadContent(const std::string referencePath, const std::string relativePath);
    template <typename T> std::shared_ptr<T> LoadContent(const std::string path);

    private:
    const std::tr2::sys::path _basePath;
    std::unordered_map<std::tuple<std::type_index, std::string>, std::shared_ptr<void>> _loadedContent;

    template <typename T> std::shared_ptr<T> LoadContent(const LoadOperation& operation);
    template <typename T> const std::string ContentExtension( );
  };

  template <typename T> std::shared_ptr<T> ContentManager::LoadContent(const std::string referencePath, const std::string relativePath) {
    auto finalPath = std::tr2::sys::path(referencePath);
    finalPath.remove_filename( );

    auto relative = std::tr2::sys::path(relativePath);
    for (auto it = relative.begin( ); it != relative.end( ); ++it) {
      if (*it == "..") {
        if (finalPath.empty( ))
          throw EngineException("Can not navigate out of content folder: " + relativePath, ErrorCode::CONTENT_INVALID_PATH);
        finalPath = finalPath.parent_path( );
      } else if (*it == ".") {

      } else {
        finalPath /= *it;
      }
    }

    return LoadContent<T>(finalPath.string( ));
  }

  template <typename T> std::shared_ptr<T> ContentManager::LoadContent(const std::string path) {
    auto finalPath = _basePath;
    auto addPath = std::tr2::sys::path(path + ContentExtension<T>( ));
    auto fixedPath = std::tr2::sys::path( );

    for (auto it = addPath.begin( ); it != addPath.end( ); ++it) {
      if (*it == "..") {
        if (fixedPath.empty( ))
          throw EngineException("Can not navigate out of content folder: " + path, ErrorCode::CONTENT_INVALID_PATH);
        finalPath = finalPath.parent_path( );
        fixedPath = fixedPath.parent_path( );
      } else if (*it == ".") {

      } else {
        finalPath /= *it;
        fixedPath /= *it;
      }
    }
    auto fullPath = finalPath.string( );
    auto relativePath = fixedPath.string( );

    auto key = std::tuple<std::type_index, std::string>(std::type_index(typeid(T)), fullPath);
    auto value = _loadedContent.find(key);

    if (value == _loadedContent.end( )) {
      auto stream = std::make_shared<std::ifstream>(fullPath, std::ios::in);
      if (!stream || !stream->is_open( )) {
        throw EngineException("Unable to load content from file: " + relativePath, ErrorCode::CONTENT_NOT_FOUND);
      }
      auto operation = LoadOperation(*this, relativePath, fullPath, stream);
      auto content = LoadContent<T>(operation);
      _loadedContent[key] = content;
      return content;
    } else {
      return std::static_pointer_cast<T>(value->second);
    }
  }

}