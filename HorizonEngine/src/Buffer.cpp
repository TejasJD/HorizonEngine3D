#include "HorizonEngine/Buffer.h"

#include <algorithm>
#include <memory>
#include <optional>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace Hzn
{

VertexBuffer::VertexBuffer(Key, float const *data, size_t const dataSizeInBytes, BufferLayout const &layout)
    : mLayout(layout)
{
    glCreateBuffers(1, &mRendererID);
    SPDLOG_INFO("VertexBuffer::{}, ID: {}", __func__, RendererID());
    glNamedBufferData(mRendererID, dataSizeInBytes, data, GL_STATIC_DRAW);
}

auto VertexBuffer::Cleanup() const -> void
{
    if (RendererID())
    {
        SPDLOG_INFO("VertexBuffer::{}, ID: {}", __func__, RendererID());
        glDeleteBuffers(1, &mRendererID);
    }
}

VertexBuffer::~VertexBuffer()
{
    Cleanup();
}

auto ElementBuffer::Cleanup() const -> void
{
    if (RendererID())
    {
        SPDLOG_INFO("ElementBuffer::{}, ID: {}", __func__, RendererID());
        glDeleteBuffers(1, &mRendererID);
    }
}

ElementBuffer::ElementBuffer(Key, uint32_t const *data, size_t const dataSizeInBytes)
{
    glCreateBuffers(1, &mRendererID);
    SPDLOG_INFO("ElementBuffer::{}, ID: {}", __func__, RendererID());
    glNamedBufferData(mRendererID, dataSizeInBytes, data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
    Cleanup();
}

} // namespace Hzn