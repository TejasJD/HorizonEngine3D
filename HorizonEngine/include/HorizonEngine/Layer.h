#ifndef HORIZON_ENGINE_LAYER_H
#define HORIZON_ENGINE_LAYER_H

#include "HorizonEngine/App.h"

#include <cassert>
#include <memory>
#include <type_traits>

namespace Hzn
{

class Layer
{
    friend class App;

  public:
    template <typename DerivedLayer, typename... Args>
        requires std::is_base_of_v<Layer, DerivedLayer>
    static auto Create(Args &&...args) -> std::shared_ptr<Layer>
    {
        return std::make_shared<DerivedLayer>(std::forward<Args>(args)...);
    }

    Layer() = default;

    virtual ~Layer() = default;

    auto AppRef() -> App &
    {
        return *mApp;
    }

    auto AppRef() const -> App const &
    {
        return *mApp;
    }

  private:
    virtual auto RenderImGui() -> void = 0;

    virtual auto OnAdd() -> bool = 0;

    virtual auto OnUpdate() -> void = 0;

    virtual auto OnRemove() -> void = 0;

    App *mApp{nullptr};
};

} // namespace Hzn

#endif // HORIZON_ENGINE_LAYER_H