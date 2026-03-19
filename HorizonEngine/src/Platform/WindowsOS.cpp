#include "WindowsOS.h"

#include <ShObjIdl.h>
#include <Windows.h>
#include <atlbase.h>
#include <spdlog/spdlog.h>

#include <filesystem>

namespace Hzn::Platform
{

auto Windows::Initialize() -> bool
{
    SPDLOG_INFO("Windows::{}", __func__);
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        return true;
    }

    mInitialized = true;
    return false;
}

auto Windows::Cleanup() -> void
{
    SPDLOG_INFO("Windows::{}", __func__);
    CoUninitialize();
    mInitialized = false;
}

Windows::~Windows()
{
    SPDLOG_INFO("Windows::{}", __func__);
    if (mInitialized)
    {
        Cleanup();
    }
}

auto Windows::GetFilePathFromDialog() -> std::optional<std::filesystem::path>
{
    SPDLOG_INFO("Windows::{}", __func__);
    HRESULT hr{0};
    if (!mFileDialog)
    {
        hr = mFileDialog.CoCreateInstance(__uuidof(FileOpenDialog));
        if (FAILED(hr))
        {
            SPDLOG_ERROR("Failed to create instance");
            return std::nullopt;
        }
    }

    assert(mFileDialog != nullptr);

    hr = mFileDialog->Show(nullptr);
    if (FAILED(hr))
    {
        SPDLOG_ERROR("IFileOpenDialog::Show failed");
        return std::nullopt;
    }

    CComPtr<IShellItem> pShellItem;

    hr = mFileDialog->GetResult(&pShellItem);
    if (FAILED(hr))
    {
        SPDLOG_ERROR("IFileOpenDialog::GetResult failed");
        return std::nullopt;
    }

    LPWSTR filePathWstr{nullptr};
    hr = pShellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &filePathWstr);
    if (FAILED(hr))
    {
        SPDLOG_ERROR("IShellItem::GetResult failed");
        return std::nullopt;
    }

    assert(!!filePathWstr && "filePathWstr should not be NULL");

    return std::filesystem::path{filePathWstr};
}

} // namespace Hzn::Platform