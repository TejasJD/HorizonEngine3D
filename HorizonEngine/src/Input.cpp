#include "HorizonEngine/Input.h"

#include <cassert>
#include <functional>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace Hzn
{

auto PolledInputManager::SetKeyCallback(KeyCode const key, std::function<void(ButtonState)> const &keyCallback) -> void
{
    char const *keyName = glfwGetKeyName(GLFW_KEY_UNKNOWN, glfwGetKeyScancode(static_cast<int32_t>(key)));
    assert(!!keyName && "key name is null");
    SPDLOG_INFO("PolledInputManager::{}, Setting callback for Key: {}", __func__, keyName);
    assert(!!keyCallback && "Callback should be attached to a callback object");
    mKeyCallbacks.emplace(key, keyCallback);
}

auto PolledInputManager::ResetKeyCallback(KeyCode const key) -> void
{
    char const *keyName = glfwGetKeyName(GLFW_KEY_UNKNOWN, glfwGetKeyScancode(static_cast<int32_t>(key)));
    assert(!!keyName && "key name is null");
    SPDLOG_INFO("PolledInputManager::{}, Erasing callback for Key: {}", __func__, keyName);
    mKeyCallbacks.erase(key);
}

auto PolledInputManager::SetMouseButtonCallback(MouseCode const mouseButton,
                                                std::function<void(ButtonState)> const &mouseButtonCallback) -> void
{
    SPDLOG_INFO("PolledInputManager::{}", __func__);
    assert(!!mouseButtonCallback && "Callback should be attached to a callback object");
    mMouseButtonCallbacks.emplace(mouseButton, mouseButtonCallback);
}

auto PolledInputManager::ResetMouseButtonCallback(MouseCode const mouseButton) -> void
{
    SPDLOG_INFO("PolledInputManager::{}", __func__);
    mMouseButtonCallbacks.erase(mouseButton);
}

auto PolledInputManager::SetScrollCallback(std::function<void(double, double)> const &scrollCallback) -> void
{
    SPDLOG_INFO("PolledInputManager::{}", __func__);
    assert(!mWindow.expired() && "Function shouldn't be invoked when the window pointer is NULL");
    auto window = mWindow.lock();
    assert(!!window);

    window->SetScrollCallback(scrollCallback);
}

auto PolledInputManager::ResetScrollCallback() -> void
{
    SPDLOG_INFO("PolledInputManager::{}", __func__);
    assert(!mWindow.expired() && "Function shouldn't be invoked when the window pointer is NULL");
    auto window = mWindow.lock();
    assert(!!window);

    window->SetScrollCallback(std::function<void(double, double)>{});
}

auto PolledInputManager::SetWindowPtr(std::shared_ptr<Window> const &window) -> void
{
    SPDLOG_INFO("PolledInputManager::{}", __func__);
    assert(!!window && "Window pointer should not be NULL");
    mWindow = window;
}

auto PolledInputManager::InvokeCallbacks() const -> void
{
    assert(!mWindow.expired());
    auto window = mWindow.lock();

    assert(!!window);
    assert(window->GetHandle().has_value());

    GLFWwindow *glfwWindow = std::any_cast<GLFWwindow *>(window->GetHandle());

    std::for_each(mKeyCallbacks.begin(), mKeyCallbacks.end(), [this](auto const &iter) {
        auto const &[key, callback] = iter;
        ButtonState action = GetKeyState(key);
        callback(action);
    });

    std::for_each(mMouseButtonCallbacks.begin(), mMouseButtonCallbacks.end(), [this](auto const &iter) {
        auto const &[mouseButton, callback] = iter;
        ButtonState action = GetMouseButtonState(mouseButton);
        callback(action);
    });

    double xpos, ypos;
    glfwGetCursorPos(glfwWindow, &xpos, &ypos);
    if (mCursorCallback)
    {
        mCursorCallback(xpos, ypos);
    }
}

auto PolledInputManager::SetCursorCallback(std::function<void(double, double)> const &cursorCallback) -> void
{
    // assert(!mWindow.expired());
    // auto glfwWindow = std::any_cast<GLFWwindow *>(mWindow.lock()->GetHandle());
    // glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mCursorCallback = cursorCallback;
}

auto PolledInputManager::ResetCursorCallback() -> void
{
    mCursorCallback = {};
    // assert(!mWindow.expired());
    // auto glfwWindow = std::any_cast<GLFWwindow *>(mWindow.lock()->GetHandle());
    // glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

auto PolledInputManager::GetKeyState(KeyCode const key) const -> ButtonState
{
    assert(!mWindow.expired());
    auto glfwWindow = std::any_cast<GLFWwindow *>(mWindow.lock()->GetHandle());
    int32_t state = glfwGetKey(glfwWindow, static_cast<int32_t>(key));

    return (state == GLFW_PRESS ? ButtonState::Pressed : ButtonState::Released);
}

auto PolledInputManager::GetMouseButtonState(MouseCode const button) const -> ButtonState
{
    assert(!mWindow.expired());
    auto glfwWindow = std::any_cast<GLFWwindow *>(mWindow.lock()->GetHandle());
    int32_t state = glfwGetMouseButton(glfwWindow, static_cast<int32_t>(button));

    return (state == GLFW_PRESS ? ButtonState::Pressed : ButtonState::Released);
}

} // namespace Hzn