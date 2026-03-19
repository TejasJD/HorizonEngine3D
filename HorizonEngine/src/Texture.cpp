#include "HorizonEngine/Texture.h"

#include <cstdint>
#include <filesystem>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>

namespace Hzn
{

auto Texture::Create(std::filesystem::path const &path) -> std::shared_ptr<Texture>
{
    SPDLOG_INFO("Texture::{}, filepath: {}", __func__, path.string());

    stbi_set_flip_vertically_on_load(1);
    int32_t width = 0, height = 0, components = 0;

    int32_t result = stbi_info(path.string().c_str(), &width, &height, &components);
    if (!result)
    {
        SPDLOG_ERROR("Failed to query file information: {}", path.string());
        return nullptr;
    }

    SPDLOG_INFO("Queried file info, Width: {}, Height: {}, Components: {}", width, height, components);

    auto data = stbi_load(path.string().c_str(), &width, &height, &components, components);
    if (!data)
    {
        char const *failureReason = stbi_failure_reason();
        SPDLOG_ERROR("Failed to load texture image. Reason: {}", failureReason);
        return nullptr;
    }

    SPDLOG_INFO("Loaded file info, Width: {}, Height: {}, Components: {}", width, height, components);

    int32_t internalFormat = 0, dataFormat = 0;
    if (components == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (components == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    else
    {
        SPDLOG_ERROR("Component value not support yet. Components: {}", components);
        stbi_image_free(data);
        return nullptr;
    }

    uint32_t textureID = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(textureID, 1, internalFormat, width, height);
    glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    auto texture = std::make_shared<Texture>(Key{});
    texture->mWidth = width;
    texture->mHeight = height;
    texture->mComponents = components;
    texture->mTextureID = textureID;

    stbi_image_free(data);
    return texture;
}

auto Texture::Cleanup() -> void
{
    if (mTextureID)
    {
        SPDLOG_INFO("Texture::{}, ID: {}", __func__, mTextureID);
        glDeleteTextures(1, &mTextureID);
    }
}

Texture::~Texture()
{
    Cleanup();
}

auto Texture::Bind(uint32_t slot) const -> void
{
    glBindTextureUnit(slot, mTextureID);
}

} // namespace Hzn