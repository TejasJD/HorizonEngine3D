#ifndef HORIZON_ENGINE_UTILITY_H
#define HORIZON_ENGINE_UTILITY_H

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <optional>

namespace Hzn
{

enum class Type
{
    None = 0,
    Bool,
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Vec2i,
    Vec3i,
    Vec4i,
    Mat3,
    Mat4,
    Mat3i,
    Mat4i
};

namespace Utils
{

inline static auto ElementCount(Type type) -> int32_t
{
    switch (type)
    {
    case Hzn::Type::None:
        return 0;
    case Hzn::Type::Bool:
    case Hzn::Type::Int:
    case Hzn::Type::Float:
        return 1;
    case Hzn::Type::Vec2:
    case Hzn::Type::Vec2i:
        return 2;
    case Hzn::Type::Vec3:
    case Hzn::Type::Vec3i:
        return 3;
    case Hzn::Type::Vec4:
    case Hzn::Type::Vec4i:
        return 4;
    case Hzn::Type::Mat3:
    case Hzn::Type::Mat3i:
        return 9;
    case Hzn::Type::Mat4:
    case Hzn::Type::Mat4i:
        return 16;
    default:
        assert(false && "unexpected data type");
    }

    return std::numeric_limits<int32_t>::max();
}

inline static auto SizeOfType(Type type) -> int32_t
{
    switch (type)
    {
    case Hzn::Type::None:
        return 0;
    case Hzn::Type::Bool:
        return sizeof(bool);
    case Hzn::Type::Int:
    case Hzn::Type::Vec2i:
    case Hzn::Type::Vec3i:
    case Hzn::Type::Vec4i:
    case Hzn::Type::Mat3i:
    case Hzn::Type::Mat4i:
        return ElementCount(type) * sizeof(int);
    case Hzn::Type::Float:
    case Hzn::Type::Vec2:
    case Hzn::Type::Vec3:
    case Hzn::Type::Vec4:
    case Hzn::Type::Mat3:
    case Hzn::Type::Mat4:
        return ElementCount(type) * sizeof(float);
    default:
        assert(false && "unexpected data type");
    }

    return std::numeric_limits<int32_t>::max();
}

auto ReadFromFileIntoString(std::filesystem::path const &path) -> std::optional<std::string>;

} // namespace Utils

} // namespace Hzn

#endif // HORIZON_ENGINE_UTILITY_H