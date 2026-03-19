#ifndef HZN_PLATFORM_WINDOWS_H
#define HZN_PLATFORM_WINDOWS_H

#include "HorizonEngine/Platform.h"

#include <ShObjIdl.h>
#include <atlbase.h>
#include <filesystem>

namespace Hzn::Platform
{

class Windows : public Hzn::OS
{
  public:
    Windows() = default;
    virtual ~Windows();

    virtual auto Initialize() -> bool override;
    virtual auto Cleanup() -> void override;
    virtual auto GetFilePathFromDialog() -> std::optional<std::filesystem::path> override;

  private:
    bool mInitialized{false};
    CComPtr<IFileDialog> mFileDialog{nullptr};
};

} // namespace Hzn::Platform

#endif // HZN_PLATFORM_WINDOWS_H