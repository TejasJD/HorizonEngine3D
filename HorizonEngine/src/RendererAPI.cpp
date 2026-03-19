#include "HorizonEngine/RendererAPI.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace Hzn
{

static auto GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                       const GLchar *message, const void *userParam) -> void
{
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        SPDLOG_WARN("GL CALLBACK: {} type = {:#x}, severity = {:#x}, message = {}",
                    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
    }
}

auto RendererAPI::Info() -> void
{
    auto version = reinterpret_cast<char const *>(glGetString(GL_VERSION));
    auto renderer = reinterpret_cast<char const *>(glGetString(GL_RENDERER));
    auto vendor = reinterpret_cast<char const *>(glGetString(GL_VENDOR));

    SPDLOG_INFO("RendererAPI::{}, Vendor: {}, Renderer: {}, API version: {}", __func__, vendor, renderer, version);
}

auto RendererAPI::SetClearColor(glm::vec4 const &color) -> void
{
    glClearColor(color.r, color.g, color.b, color.a);
}

auto RendererAPI::Clear(BufferBit const bufferBitFlag) -> void
{
    uint32_t bitFlag = 0;
    bitFlag |= (bufferBitFlag & BufferBit::Color) == BufferBit::Color ? GL_COLOR_BUFFER_BIT : 0;
    bitFlag |= (bufferBitFlag & BufferBit::Depth) == BufferBit::Depth ? GL_DEPTH_BUFFER_BIT : 0;
    glClear(bitFlag);
}

auto RendererAPI::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height) -> void
{
    glViewport(x, y, width, height);
}

auto RendererAPI::Enable(EnableOptions const options) -> void
{
    switch (options)
    {
    case EnableOptions::None:
        return;
    case EnableOptions::DebugOutput: {
        glDebugMessageCallback(MessageCallback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        return;
    }
    case EnableOptions::DepthTest: {
        glEnable(GL_DEPTH_TEST);
        return;
    }
    default:
        SPDLOG_ERROR("Unexcepted EnableOptions");
        assert(false && "Unexcepted EnableOptions");
    }
}

auto RendererAPI::DrawElements(std::shared_ptr<VertexArray> const &vertexArray, std::shared_ptr<Shader> const &shader,
                               size_t const count) -> void
{
    assert(!!vertexArray && !!shader);
    vertexArray->Bind();
    shader->Bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    shader->Unbind();
    vertexArray->Unbind();
}

} // namespace Hzn