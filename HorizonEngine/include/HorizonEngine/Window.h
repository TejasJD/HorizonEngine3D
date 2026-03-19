#ifndef HORIZON_ENGINE_WINDOW_H
#define HORIZON_ENGINE_WINDOW_H

#include <any>
#include <functional>
#include <memory>
#include <string_view>

namespace Hzn
{

class App;
class PolledInputManager;

struct WindowSpecification
{
    WindowSpecification() = default;
    WindowSpecification(int32_t width, int32_t height, char const *title) : Width(width), Height(height), Title(title)
    {
    }

    int32_t Width{800};
    int32_t Height{600};
    std::string_view Title{"HorizonEngine"};
};

class Window
{
    friend class App;
    friend class PolledInputManager;

  private:
    class Key
    {
        friend class Window;
    };

    static auto Create(WindowSpecification const &spec) -> std::unique_ptr<Window>;

    class Data
    {
      public:
        friend class PolledInputMgr;
        Data(WindowSpecification const &spec) : Spec(spec)
        {
        }

        WindowSpecification Spec;
        std::function<void(double, double)> ScrollCallback;
    };

  public:
    Window(Key, WindowSpecification const &spec) : mData(spec)
    {
    }

    Window(Window const &) = delete;

    Window &operator=(Window const &) = delete;

    Window(Window &&) = delete;

    Window &operator=(Window &&) = delete;

    ~Window();

    auto ShouldClose() const -> bool;
    auto PollEvents() const -> void;
    auto SwapBuffers() const -> void;

    auto Width() const -> int32_t
    {
        return mData.Spec.Width;
    }

    auto Height() const -> int32_t
    {
        return mData.Spec.Height;
    }

    auto Title() const -> char const *
    {
        return mData.Spec.Title.data();
    }

    auto AspectRatio() const -> float
    {
        return static_cast<float>(mData.Spec.Width) / mData.Spec.Height;
    }

    auto GetHandle() const -> std::any
    {
        return mHandle;
    }

  private:
    auto Cleanup() -> void;

    auto SetCallbacks() -> void;

    auto SetScrollCallback(std::function<void(double, double)> const &scrollCallback) -> void;

    Data mData;
    std::any mHandle;
};

} // namespace Hzn

#endif // HORIZON_ENGINE_WINDOW_H!
