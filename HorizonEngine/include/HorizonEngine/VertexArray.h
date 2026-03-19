#ifndef HORIZON_ENGINE_VERTEX_ARRAY_H
#define HORIZON_ENGINE_VERTEX_ARRAY_H

#include "HorizonEngine/Buffer.h"

#include <memory>

namespace Hzn
{

class VertexArray
{
  private:
    class Key
    {
        friend class VertexArray;
    };

  public:
    static auto Create() -> std::shared_ptr<VertexArray>
    {
        return std::make_shared<VertexArray>(Key{});
    }

    VertexArray(Key);

    VertexArray(VertexArray const &) = delete;

    VertexArray(VertexArray &&) = delete;

    VertexArray &operator=(VertexArray const &) = delete;

    VertexArray &operator=(VertexArray &&) = delete;

    ~VertexArray();

    auto Cleanup() -> void;

    auto RendererID() const -> uint32_t
    {
        return mRendererID;
    }

    auto AddVertexBuffer(std::shared_ptr<VertexBuffer> const &vertexBuffer) -> void;

    auto SetElementBuffer(std::shared_ptr<ElementBuffer> const &elementBuffer) -> void;

    auto Bind() const -> void;

    auto Unbind() const -> void;

  private:
    std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
    std::shared_ptr<ElementBuffer> mElementBuffer;
    uint32_t mRendererID{0};
};

} // namespace Hzn

#endif // HORIZON_ENGINE_VERTEX_ARRAY_H
