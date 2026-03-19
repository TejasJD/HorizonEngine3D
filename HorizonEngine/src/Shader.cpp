#include "HorizonEngine/Shader.h"
#include "HorizonEngine/Utility.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

namespace Hzn
{

namespace Utils
{

static auto ToGLShaderType(ShaderType type) -> int32_t
{
    switch (type)
    {
    case Hzn::ShaderType::None:
        return GL_NONE;
    case Hzn::ShaderType::Vertex:
        return GL_VERTEX_SHADER;
    case Hzn::ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    default:
        assert(false && "Unexpected shader type");
    }

    return std::numeric_limits<int32_t>::max();
}

static auto ToGLShaderStageBit(ShaderType type) -> int32_t
{
    switch (type)
    {
    case Hzn::ShaderType::None:
        return GL_NONE;
    case Hzn::ShaderType::Vertex:
        return GL_VERTEX_SHADER_BIT;
    case Hzn::ShaderType::Fragment:
        return GL_FRAGMENT_SHADER_BIT;
    default:
        assert(false && "Unexpected shader type");
    }

    return std::numeric_limits<int32_t>::max();
}

} // namespace Utils

auto Shader::Create(std::initializer_list<ShaderSource> const &shaderSources) -> std::shared_ptr<Shader>
{
    SPDLOG_INFO("Shader::{}", __func__);

    std::unordered_map<ShaderType, uint32_t> shaderPrograms;
    bool allShaderProgramsCreated =
        std::all_of(shaderSources.begin(), shaderSources.end(), [&](ShaderSource const &shaderSource) {
            auto shaderSourceOpt = Utils::ReadFromFileIntoString(shaderSource.FilePath);
            if (!shaderSourceOpt.has_value())
            {
                return false;
            }

            char const *shaderSourceCstr = shaderSourceOpt->c_str();

            int32_t shaderProgram =
                glCreateShaderProgramv(Utils::ToGLShaderType(shaderSource.Type), 1, &shaderSourceCstr);

            int32_t programCreated = 0;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programCreated);
            if (!programCreated)
            {
                int32_t infoLogLength = 0;
                glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
                std::string infoLog(infoLogLength, '\0');

                glGetProgramInfoLog(shaderProgram, infoLogLength, &infoLogLength, infoLog.data());
                SPDLOG_ERROR("Failed to create shader program. Reason: {}", infoLog);
                return false;
            }

            shaderPrograms.emplace(shaderSource.Type, shaderProgram);

            return true;
        });

    if (!allShaderProgramsCreated)
    {
        return nullptr;
    }

    assert((shaderPrograms.size() == shaderSources.size()) &&
           "Actual shader program count doesn't match expected count");

    uint32_t pipeline = 0;
    glCreateProgramPipelines(1, &pipeline);
    std::for_each(shaderPrograms.begin(), shaderPrograms.end(), [pipeline](auto const &iter) {
        auto &&[shaderType, shaderProgram] = iter;
        glUseProgramStages(pipeline, Utils::ToGLShaderStageBit(shaderType), shaderProgram);
    });

    auto shader = std::make_shared<Shader>(Key{});
    shader->mPipelineID = pipeline;
    shader->mShaderPrograms = std::move(shaderPrograms);

    SPDLOG_INFO("Shader object created successfully. Pipeline ID: {}", pipeline);
    return shader;
}

Shader::Shader(Key)
{
}

auto Shader::Cleanup() -> void
{
    if (PipelineID())
    {
        SPDLOG_INFO("Shader::{}, Pipeline ID: {}", __func__, mPipelineID);
        glDeleteProgramPipelines(1, &mPipelineID);
    }

    std::for_each(mShaderPrograms.begin(), mShaderPrograms.end(), [this](auto const &iter) {
        auto const shaderProgram = iter.second;
        SPDLOG_INFO("Deleting shader programs associated with the pipeline. ID: {}, Program ID: {}", PipelineID(),
                    shaderProgram);
        glDeleteProgram(shaderProgram);
    });
}

Shader::~Shader()
{
    Cleanup();
}

auto Shader::Bind() const -> void
{
    SPDLOG_TRACE("Shader::{}, Pipeline ID: {}", __func__, PipelineID());
    glBindProgramPipeline(PipelineID());
}

auto Shader::Unbind() const -> void
{
    SPDLOG_TRACE("Shader::{}, Pipeline ID: {}", __func__, PipelineID());
    glBindProgramPipeline(0);
}

template <> auto Shader::SetUniform(ShaderType const type, glm::mat4 const &value, char const *name) -> void
{
    if (!mShaderPrograms.contains(type))
    {
        SPDLOG_WARN("Shader::{}, doesn't contain program for specified type. PipelineID: {}", __func__, PipelineID());
    }

    uint32_t shaderProgram = mShaderPrograms.at(type);
    auto location = glGetUniformLocation(shaderProgram, name);
    glProgramUniformMatrix4fv(shaderProgram, location, 1, false, glm::value_ptr(value));
}

} // namespace Hzn
