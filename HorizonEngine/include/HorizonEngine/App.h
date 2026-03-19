#ifndef HORIZON_ENGINE_APP_H
#define HORIZON_ENGINE_APP_H

#include "HorizonEngine/Input.h"
#include "HorizonEngine/Platform.h"
#include "HorizonEngine/RendererAPI.h"
#include "HorizonEngine/Window.h"

#include <cassert>
#include <list>
#include <memory>
#include <type_traits>

namespace Hzn
{

class Layer;
class App;

class Detail
{
    friend class App;
    static auto InitializeApp() -> bool;
    static auto InitializePlatform() -> std::shared_ptr<OS>;
};

struct AppSpecification
{
    AppSpecification() = default;

    explicit AppSpecification(WindowSpecification const &windowSpec) : WindowSpec(windowSpec)
    {
    }

    WindowSpecification WindowSpec{};
    bool EnableDebugUI{true};
};

class App
{
    friend class Detail;

  public:
    virtual auto Initialize() -> bool = 0;

    template <typename Derived, typename... Args>
        requires std::is_base_of_v<App, Derived>
    static auto Create(AppSpecification const &spec, Args &&...args) -> std::unique_ptr<App>
    {
        std::unique_ptr<App> app = std::make_unique<Derived>(std::forward<Args>(args)...);
        assert(!!app && "Initial pointer creation for App is not expected to fail");

        if (!Detail::InitializeApp())
        {
            return nullptr;
        }

        app->mWindow = Window::Create(spec.WindowSpec);

        if (!app->mWindow)
        {
            return nullptr;
        }

        app->mPlatform = Detail::InitializePlatform();

        if (!app->mPlatform)
        {
            return nullptr;
        }

        app->mInputManager = PolledInputManager::Create();
        app->mInputManager->SetWindowPtr(app->mWindow);
        app->mWindow->SetCallbacks();
        RendererAPI::Info();

        if (!app->InitializeImGui())
        {
            return nullptr;
        }

        if (!app->Initialize())
        {
            return nullptr;
        }

        return app;
    }

    App()
    {
    }

    virtual ~App();

    App(App const &) = delete;

    App(App &&) = delete;

    App &operator=(App const &) = delete;

    App &operator=(App &&) = delete;

    auto Run() -> void;

    auto PlatformRef() const -> OS &
    {
        return *mPlatform;
    }

    auto WindowRef() const -> Window const &
    {
        return *mWindow;
    }

    auto InputManagerRef() const -> PolledInputManager &
    {
        return *mInputManager;
    }

    auto DeltaTime() const -> float
    {
        return mDeltaTime;
    }

    auto AddLayer(std::shared_ptr<Layer> const &layer) -> bool;

    auto RemoveLayer(std::shared_ptr<Layer> const &layer) -> void;

  private:
    auto InitializeImGui() const -> bool;

    auto CleanupImGui() -> void;

    auto FrameBeginImGui() -> void;

    auto FrameEndImGui() -> void;

    std::shared_ptr<OS> mPlatform;
    std::shared_ptr<PolledInputManager> mInputManager;
    std::shared_ptr<Window> mWindow;
    std::list<std::shared_ptr<Layer>> mLayers;
    double mPreviousTime{0.0};
    float mDeltaTime{0.0f};
    bool mImGuiInitialized{false};
};

} // namespace Hzn

#endif // HORIZON_ENGINE_APP_H