#ifndef HORIZON_ENGINE_SHADER_H
#define HORIZON_ENGINE_SHADER_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <unordered_map>

namespace Hzn
{

enum class ShaderType
{
    None = 0,
    Vertex,
    Fragment
};

struct ShaderSource
{
    ShaderSource(ShaderType type, std::filesystem::path const &path) : Type(type), FilePath(path)
    {
    }

    ShaderType Type;
    std::filesystem::path FilePath;
};

class Shader
{
  private:
    class Key
    {
        friend class Shader;
    };

  public:
    static auto Create(std::initializer_list<ShaderSource> const &shaderSources) -> std::shared_ptr<Shader>;

    Shader(Key);

    Shader(Shader const &) = delete;

    Shader(Shader &&) = delete;

    Shader &operator=(Shader const &) = delete;

    Shader &operator=(Shader &&) = delete;

    ~Shader();

    auto Cleanup() -> void;

    auto PipelineID() const -> uint32_t
    {
        return mPipelineID;
    }

    auto Bind() const -> void;

    auto Unbind() const -> void;

    template <typename T> auto SetUniform(ShaderType Type, T const &value, char const *name) -> void;

  private:
    std::unordered_map<ShaderType, uint32_t> mShaderPrograms;
    uint32_t mPipelineID{0};
};

} // namespace Hzn

#endif // HORIZON_ENGINE_SHADER_H
