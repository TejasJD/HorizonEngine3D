#ifndef HORIZON_ENGINE_PLATFORM_H
#define HORIZON_ENGINE_PLATFORM_H

#include <filesystem>
#include <memory>
#include <type_traits>

namespace Hzn
{

class OS
{
  public:
    template <typename Derived, typename... Args>
        requires std::is_base_of_v<OS, Derived>
    static auto Create(Args &&...args) -> std::shared_ptr<OS>
    {
        auto os = std::make_shared<Derived>(std::forward<Args>(args)...);
        if (!os->Initialize())
        {
            return nullptr;
        }

        return os;
    }

    OS() = default;
    OS(OS const &) = delete;
    OS(OS &&) = delete;
    OS &operator=(OS const &) = delete;
    OS &operator=(OS &&) = delete;

    virtual ~OS() = default;
    virtual auto Initialize() -> bool = 0;
    virtual auto Cleanup() -> void = 0;
    virtual auto GetFilePathFromDialog() -> std::optional<std::filesystem::path> = 0;

  private:
};

} // namespace Hzn

#endif // HORIZON_ENGINE_PLATFORM_H