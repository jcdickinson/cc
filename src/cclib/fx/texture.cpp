#include "stdafx.h"
#include "texture.h"

#include <memory>

#include <gl/glew.h>
#include <gl/glfw3.h>

#include "../content/contentmanager.h"
#include "../logging.h"

using namespace std;

namespace fx
{
  Texture::Texture(const uint32_t id, const uint32_t width, const uint32_t height, const uint32_t linearSize, const uint32_t mipmapCount) :
    _id(id),
    _width(width),
    _height(height),
    _linearSize(linearSize),
    _mipMapCount(mipmapCount) {

  }

  Texture::~Texture() {

  }

  const uint32_t Texture::Id() { return _id; }
  const uint32_t Texture::Width() { return _width; }
  const uint32_t Texture::Height() { return _height; }
  const uint32_t Texture::LinearSize() { return _linearSize; }
  const uint32_t Texture::MipMapCount() { return _mipMapCount; }
}

namespace content
{
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

  template<> shared_ptr<fx::Texture> ContentManager::LoadContent<fx::Texture>(const LoadOperation& operation) {
    uint8_t header[124];

    LOG(DEBUG) << L"Loading texture " << operation.Path << L" ...";
    operation.Data->read(reinterpret_cast<char*>(&header[0]), 4);
    if (strncmp(reinterpret_cast<char*>(&header[0]), "DDS ", 4) != 0) {
      LOG(ERROR) << L"Bad texture header " << operation.Path << L" .";
      throw EngineException("Failed to load texture: " + operation.Path, ErrorCode::FX_TEXTURE_LOAD_FAILURE);
    }

    operation.Data->read(reinterpret_cast<char*>(&header[0]), 124);
    auto height = *(uint32_t*)&(header[8]);
    auto width = *(uint32_t*)&(header[12]);
    auto linearSize = *(uint32_t*)&(header[16]);
    auto mipMapCount = *(uint32_t*)&(header[24]);
    auto fourCC = *(uint32_t*)&(header[80]);
    uint32_t format;

    switch (fourCC)
    {
    case FOURCC_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
    case FOURCC_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
    case FOURCC_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
    default:
      LOG(ERROR) << L"Unsupported FOURCC: " << fourCC << ": " << operation.Path << L" .";
      throw EngineException("Failed to load texture: " + operation.Path, ErrorCode::FX_TEXTURE_LOAD_FAILURE);
    }

    if (linearSize > 104857600)
    {
      LOG(ERROR) << L"Texture too large " << operation.Path << L" .";
      throw EngineException("Failed to load texture: " + operation.Path, ErrorCode::FX_TEXTURE_LOAD_FAILURE);
    }

    size_t bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    auto buffer = reinterpret_cast<uint8_t*>(malloc(bufferSize));
    operation.Data->read(reinterpret_cast<char*>(buffer), bufferSize);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    uint32_t blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    size_t offset = 0;
    for (uint32_t level = 0; level < mipMapCount && (width || height); ++level)
    {
      uint32_t size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
      glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

      offset += size;
      width /= 2;
      height /= 2;
    }
    free(buffer);

    return make_shared<fx::Texture>(textureID, width, height, linearSize, mipMapCount);
  }

  template<> const string ContentManager::ContentExtension<fx::Texture>() {
    return ".dds";
  }
}