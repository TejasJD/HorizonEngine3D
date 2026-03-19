#include "HorizonEngine/Utility.h"

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

#include <spdlog/spdlog.h>

namespace Hzn
{

namespace Utils
{

auto ReadFromFileIntoString(std::filesystem::path const &path) -> std::optional<std::string>
{
    SPDLOG_INFO("Utils::{}, path: {}", __func__, path.string());
    auto file = std::ifstream{path, std::ios::binary | std::ios::ate};
    if (!file)
    {
        SPDLOG_ERROR("Failed to open file, path: {}", path.string());
        return std::nullopt;
    }

    auto dataLength = file.tellg();
    std::string contents(dataLength, '\0');

    file.seekg(0);
    file.read(contents.data(), dataLength);

    SPDLOG_INFO("Data size: {} bytes. Content:\n{}", static_cast<size_t>(dataLength), contents);
    return contents;
}

} // namespace Utils

} // namespace Hzn