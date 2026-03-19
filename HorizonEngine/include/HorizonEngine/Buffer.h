#ifndef HORIZON_ENGINE_BUFFER_H
#define HORIZON_ENGINE_BUFFER_H

#include "HorizonEngine/Utility.h"

#include <algorithm>
#include <memory>
#include <string_view>

namespace Hzn
{

struct BufferElement
{
    BufferElement(Type dataType, char const *identifier, bool normalized = false)
        : DataType(dataType), Identifier(identifier), Normalized(normalized)
    {
    }

    std::string_view Identifier;
    Type DataType;
    bool Normalized;
};

struct BufferLayout
{
    BufferLayout(std::initializer_list<BufferElement> const &layout) : Layout(layout.begin(), layout.end())
    {
        std::for_each(Layout.begin(), Layout.end(),
                      [this](auto const &element) { Stride += Utils::SizeOfType(element.DataType); });
    }

    std::vector<BufferElement> Layout;
    uint32_t Stride{0};
};

class VertexBuffer
{
  private:
    class Key
    {
        friend class VertexBuffer;
    };

  public:
    static auto Create(float const *data, size_t const dataSizeInBytes, BufferLayout const &layout)
        -> std::shared_ptr<VertexBuffer>
    {
        return std::make_shared<VertexBuffer>(Key{}, data, dataSizeInBytes, layout);
    }

    VertexBuffer(Key, float const *data, size_t const dataSizeInBytes, BufferLayout const &layout);

    VertexBuffer(VertexBuffer const &) = delete;

    VertexBuffer(VertexBuffer &&) = delete;

    VertexBuffer &operator=(VertexBuffer const &) = delete;

    VertexBuffer &operator=(VertexBuffer &&) = delete;

    ~VertexBuffer();

    auto RendererID() const -> uint32_t
    {
        return mRendererID;
    }

    auto Layout() const -> BufferLayout const &
    {
        return mLayout;
    }

    auto Cleanup() const -> void;

  private:
    BufferLayout mLayout;
    uint32_t mRendererID{0};
};

class ElementBuffer
{
  private:
    class Key
    {
        friend class ElementBuffer;
    };

  public:
    static auto Create(const uint32_t *data, size_t const dataSizeInBytes) -> std::shared_ptr<ElementBuffer>
    {
        return std::make_shared<ElementBuffer>(Key{}, data, dataSizeInBytes);
    }

    ElementBuffer(Key, const uint32_t *data, size_t const dataSizeInBytes);

    ElementBuffer(ElementBuffer const &) = delete;

    ElementBuffer(ElementBuffer &&) = delete;

    ElementBuffer &operator=(ElementBuffer const &) = delete;

    ElementBuffer &operator=(ElementBuffer &&) = delete;

    ~ElementBuffer();

    auto RendererID() const -> uint32_t
    {
        return mRendererID;
    }

    auto Cleanup() const -> void;

  private:
    uint32_t mRendererID{0};
};

} // namespace Hzn

#endif // HORIZON_ENGINE_BUFFER_H