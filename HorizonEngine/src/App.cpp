#include "HorizonEngine/App.h"

#include "HorizonEngine/Layer.h"
#include "HorizonEngine/Window.h"
#include "Platform/WindowsOS.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

namespace Hzn
{

auto Detail::InitializeApp() -> bool
{
    if (!glfwInit())
    {
        SPDLOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    return true;
}

auto Detail::InitializePlatform() -> std::shared_ptr<OS>
{
    return OS::Create<Platform::Windows>();
}

auto App::InitializeImGui() const -> bool
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    auto windowHandle = WindowRef().GetHandle();
    assert(windowHandle.has_value());
    auto glfwWindow = std::any_cast<GLFWwindow *>(windowHandle);
    assert(!!glfwWindow);

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 450 core");

    RendererAPI::SetClearColor(glm::vec4{0.0f});
    return true;
}

auto App::CleanupImGui() -> void
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

auto App::FrameBeginImGui() -> void
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

auto App::FrameEndImGui() -> void
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

auto App::Run() -> void
{
    SPDLOG_INFO("App::{}", __func__);
    mPreviousTime = glfwGetTime();
    while (!mWindow->ShouldClose())
    {
        RendererAPI::Clear(BufferBit::Color | BufferBit::Depth);
        double currentTime = glfwGetTime();
        mDeltaTime = static_cast<float>(currentTime - mPreviousTime);
        mPreviousTime = currentTime;

        mWindow->PollEvents();
        mInputManager->InvokeCallbacks();

        for (auto const &layer : mLayers)
        {
            layer->OnUpdate();
        }

        FrameBeginImGui();
        for (auto const &layer : mLayers)
        {
            layer->RenderImGui();
        }
        FrameEndImGui();

        mWindow->SwapBuffers();
    }
}

auto App::AddLayer(std::shared_ptr<Layer> const &layer) -> bool
{
    SPDLOG_INFO("App::{}", __func__);
    assert(!!layer && "Layer should not be NULL");
    layer->mApp = this;
    if (!layer->OnAdd())
    {
        SPDLOG_WARN("Failed to add layer");
        layer->mApp = nullptr;
        return false;
    }
    mLayers.push_back(layer);
    return true;
}

auto App::RemoveLayer(std::shared_ptr<Layer> const &layer) -> void
{
    SPDLOG_INFO("App::{}", __func__);
    mLayers.erase(std::find(mLayers.begin(), mLayers.end(), layer));
    layer->OnRemove();
    layer->mApp = nullptr;
}

App::~App()
{
    // Run the cleanup function for all the layers
    std::for_each(mLayers.rbegin(), mLayers.rend(), [](auto const &layer) {
        layer->OnRemove();
        layer->mApp = nullptr;
    });
    mLayers.clear();

    mPlatform->Cleanup();
    CleanupImGui();
    mWindow->Cleanup();

    SPDLOG_INFO("App::{}", __func__);
    glfwTerminate();
}

} // namespace Hzn