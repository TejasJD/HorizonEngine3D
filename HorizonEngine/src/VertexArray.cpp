#include "HorizonEngine/VertexArray.h"
#include "HorizonEngine/Buffer.h"
#include "HorizonEngine/Utility.h"

#include <cassert>
#include <limits>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace Hzn
{

namespace Utils
{

static auto ToGLType(Type dataType) -> int32_t
{
    switch (dataType)
    {
    case Hzn::Type::None:
        return GL_NONE;
    case Hzn::Type::Bool:
        return GL_BOOL;
    case Hzn::Type::Int:
    case Hzn::Type::Vec2i:
    case Hzn::Type::Vec3i:
    case Hzn::Type::Vec4i:
    case Hzn::Type::Mat3i:
    case Hzn::Type::Mat4i:
        return GL_INT;
    case Hzn::Type::Float:
    case Hzn::Type::Vec2:
    case Hzn::Type::Vec3:
    case Hzn::Type::Vec4:
    case Hzn::Type::Mat3:
    case Hzn::Type::Mat4:
        return GL_FLOAT;
    default:
        assert(false && "Unexcepted type");
        break;
    }

    return std::numeric_limits<int32_t>::max();
}

} // namespace Utils

VertexArray::VertexArray(Key)
{
    glCreateVertexArrays(1, &mRendererID);
    SPDLOG_INFO("VertexArray::{}, ID: {}", __func__, mRendererID);
}

VertexArray::~VertexArray()
{
    Cleanup();
}

auto VertexArray::Cleanup() -> void
{
    if (RendererID())
    {
        SPDLOG_INFO("VertexArray::{}, ID: {}", __func__, mRendererID);
        glDeleteVertexArrays(1, &mRendererID);
    }
}

auto VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> const &vertexBuffer) -> void
{
    SPDLOG_INFO("VertexArray::{}, VertexArray ID: {}, VertexBuffer ID: {}", __func__, mRendererID,
                vertexBuffer->RendererID());

    auto bindingIndex = static_cast<int32_t>(mVertexBuffers.size());
    auto const &layout = vertexBuffer->Layout();
    SPDLOG_INFO("Buffer layout stride: {}", layout.Stride);

    glVertexArrayVertexBuffer(RendererID(), bindingIndex, vertexBuffer->RendererID(), 0, layout.Stride);

    uint32_t location = 0;
    uint32_t offset = 0;

    std::for_each(layout.Layout.begin(), layout.Layout.end(),
                  [this, bindingIndex, &location, &offset](BufferElement const &element) {
                      SPDLOG_INFO("Location: {}, Offset: {}", location, offset);
                      glVertexArrayAttribBinding(RendererID(), location, bindingIndex);
                      glVertexArrayAttribFormat(RendererID(), location, Utils::ElementCount(element.DataType),
                                                Utils::ToGLType(element.DataType), element.Normalized, offset);
                      glEnableVertexArrayAttrib(RendererID(), location);
                      offset += Utils::SizeOfType(element.DataType);
                      ++location;
                  });

    mVertexBuffers.push_back(vertexBuffer);
}

auto VertexArray::SetElementBuffer(std::shared_ptr<ElementBuffer> const &elementBuffer) -> void
{
    SPDLOG_INFO("VertexArray::{}, VertexArray ID: {}, ElementBuffer ID: {}", __func__, mRendererID,
                elementBuffer->RendererID());
    glVertexArrayElementBuffer(RendererID(), elementBuffer->RendererID());
    mElementBuffer = elementBuffer;
}

auto VertexArray::Bind() const -> void
{
    glBindVertexArray(RendererID());
}

auto VertexArray::Unbind() const -> void
{
    glBindVertexArray(0);
}

} // namespace Hzn