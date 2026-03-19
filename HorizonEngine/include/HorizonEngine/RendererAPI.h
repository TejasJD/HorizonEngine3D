#ifndef HORIZON_ENGINE_RENDERER_API_H
#define HORIZON_ENGINE_RENDERER_API_H

#include "HorizonEngine/Shader.h"
#include "HorizonEngine/VertexArray.h"

#include <cstdint>

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace Hzn
{

enum class BufferBit : uint32_t
{
    Color = 1U << 0,
    Depth = 1U << 1
};

constexpr inline auto operator|(BufferBit const left, BufferBit const right) -> BufferBit
{
    return static_cast<BufferBit>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr inline auto operator&(BufferBit const left, BufferBit const right) -> BufferBit
{
    return static_cast<BufferBit>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

enum class EnableOptions
{
    None = 0,
    DebugOutput,
    DepthTest
};

class RendererAPI
{
  public:
    static auto Info() -> void;
    static auto SetClearColor(glm::vec4 const &clearColor) -> void;
    static auto Clear(BufferBit const bufferBitFlag) -> void;
    static auto SetViewport(int32_t x, int32_t y, int32_t width, int32_t height) -> void;
    static auto Enable(EnableOptions const options) -> void;
    static auto DrawElements(std::shared_ptr<VertexArray> const &vertexArray, std::shared_ptr<Shader> const &shader,
                             size_t const numIndices) -> void;
};

} // namespace Hzn

#endif // HORIZON_ENGINE_RENDERER_API_H