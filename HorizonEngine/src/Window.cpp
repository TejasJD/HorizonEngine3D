#include "HorizonEngine/Window.h"

#include "HorizonEngine/RendererAPI.h"

#include <cassert>
#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace Hzn
{

auto Window::Create(WindowSpecification const &spec) -> std::unique_ptr<Window>
{
    SPDLOG_INFO("Window::{}, Width: {}, Height: {}, Title: {}", __func__, spec.Width, spec.Height, spec.Title);

    auto window = std::make_unique<Window>(Key{}, spec);
    assert(!!window && "Window pointer creation shouldn't fail");

    GLFWwindow *glfwWindow = glfwCreateWindow(window->Width(), window->Height(), window->Title(), nullptr, nullptr);
    window->mHandle = glfwWindow;

    if (!glfwWindow)
    {
        SPDLOG_ERROR("Failed to create window");
        window->Cleanup();
        return nullptr;
    }

    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SPDLOG_ERROR("GLAD failed to load OpenGL functions");
        window->Cleanup();
        return nullptr;
    }

    return window;
}

auto Window::SetCallbacks() -> void
{
    SPDLOG_INFO("Window::{}", __func__);
    assert(mHandle.has_value() && "Unexpected internal window pointer value");

    GLFWwindow *glfwWindow = std::any_cast<GLFWwindow *>(mHandle);
    glfwSetWindowUserPointer(glfwWindow, &mData);

    glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow *window, int32_t width, int32_t height) {
        auto *spec = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));
        spec->Spec.Width = width;
        spec->Spec.Height = height;
        RendererAPI::SetViewport(0, 0, width, height);
    });
}

auto Window::SetScrollCallback(std::function<void(double, double)> const &scrollCallback) -> void
{
    SPDLOG_INFO("Window::{}, Setting scoll callback: {}", __func__, !!scrollCallback);
    GLFWwindow *glfwWindow = std::any_cast<GLFWwindow *>(mHandle);
    if (!scrollCallback)
    {
        // Reset the target bound to the std::function object of mData.ScrollCallback.
        glfwSetScrollCallback(glfwWindow, nullptr);
        mData.ScrollCallback = decltype(scrollCallback){};
        return;
    }

    mData.ScrollCallback = scrollCallback;
    glfwSetScrollCallback(glfwWindow, [](GLFWwindow *window, double xOffset, double yOffset) {
        auto spec = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));
        assert(!!spec->ScrollCallback && "Scroll callback function object is expected to be valid");
        spec->ScrollCallback(xOffset, yOffset);
    });
}

auto Window::ShouldClose() const -> bool
{
    GLFWwindow *window = std::any_cast<GLFWwindow *>(mHandle);
    return glfwWindowShouldClose(window);
}

auto Window::PollEvents() const -> void
{
    glfwPollEvents();
}

auto Window::SwapBuffers() const -> void
{
    GLFWwindow *window = std::any_cast<GLFWwindow *>(mHandle);
    glfwSwapBuffers(window);
}

auto Window::Cleanup() -> void
{
    if (mHandle.has_value())
    {
        SPDLOG_INFO("Window::{}", __func__);
        GLFWwindow *glfwWindow = std::any_cast<GLFWwindow *>(mHandle);
        glfwDestroyWindow(glfwWindow);
        mHandle.reset();
    }
}

Window::~Window()
{
    Cleanup();
}

} // namespace Hzn