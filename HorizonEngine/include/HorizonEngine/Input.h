#ifndef HORIZON_ENGINE_INPUT_H
#define HORIZON_ENGINE_INPUT_H

#include "HorizonEngine/KeyCodes.h"
#include "HorizonEngine/MouseCodes.h"
#include "HorizonEngine/Window.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace Hzn
{

enum class ButtonState
{
    Released = 0,
    Pressed
};

class App;

class PolledInputManager
{
    // TODO: Keep things simple, later we can only make specific friend function
    friend class App;

  private:
    class Key
    {
        friend class PolledInputManager;
    };

  public:
    static auto Create() -> std::shared_ptr<PolledInputManager>
    {
        return std::make_shared<PolledInputManager>(Key{});
    }

    PolledInputManager(Key)
    {
    }

    ~PolledInputManager() = default;

    PolledInputManager(PolledInputManager const &) = delete;

    PolledInputManager &operator=(PolledInputManager const &) = delete;

    PolledInputManager(PolledInputManager &&) = delete;

    PolledInputManager &operator=(PolledInputManager &&) = delete;

    auto GetKeyState(KeyCode const key) const -> ButtonState;

    auto GetMouseButtonState(MouseCode const mouse) const -> ButtonState;

    auto SetKeyCallback(KeyCode const key, std::function<void(ButtonState)> const &keyCallback) -> void;

    auto ResetKeyCallback(KeyCode const key) -> void;

    auto SetMouseButtonCallback(MouseCode const mouseButton,
                                std::function<void(ButtonState)> const &mouseButtonCallback) -> void;

    auto ResetMouseButtonCallback(MouseCode const mouseButton) -> void;

    auto SetScrollCallback(std::function<void(double, double)> const &scrollCallback) -> void;

    auto ResetScrollCallback() -> void;

    auto InvokeCallbacks() const -> void;

    auto SetCursorCallback(std::function<void(double, double)> const &cursorCallback) -> void;

    auto ResetCursorCallback() -> void;

  private:
    auto SetWindowPtr(std::shared_ptr<Window> const &window) -> void;

    std::weak_ptr<Window> mWindow;
    std::unordered_map<KeyCode, std::function<void(ButtonState)>> mKeyCallbacks;
    std::unordered_map<MouseCode, std::function<void(ButtonState)>> mMouseButtonCallbacks;
    std::function<void(double, double)> mCursorCallback;
};

} // namespace Hzn

#endif // HORIZON_ENGINE_INPUT_H