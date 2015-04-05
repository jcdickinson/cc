#include "stdafx.h"
#include "contentmanager.h"

namespace content {
  template<> std::shared_ptr<std::string> ContentManager::LoadContent<std::string>(const LoadOperation& operation) {
    return std::make_unique<std::string>("nada");
  }

  template<> const std::string ContentManager::ContentExtension<std::string>( ) {
    return ".glsl";
  }
}