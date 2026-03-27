#ifndef HORIZON_EDITOR_GAME_LAYER_H
#define HORIZON_EDITOR_GAME_LAYER_H

#include "EditorUI.h"

#include "HorizonEngine/Buffer.h"
#include "HorizonEngine/Camera.h"
#include "HorizonEngine/CameraController.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/Shader.h"
#include "HorizonEngine/VertexArray.h"

#include <memory>
#include <optional>
#include <unordered_set>

#include <glm/glm.hpp>

namespace HznEditor
{

struct ColorVertex
{
    glm::vec3 aPosition{0.0f};
    glm::vec3 aColor{0.0f};
};

struct TextureVertex
{
    glm::vec3 aPosition{0.0f};
    glm::vec2 aTexCoord{0.0f};
};

using Face = glm::uvec3;

class GameLayer : public Hzn::Layer
{
  public:
    GameLayer() = default;

    virtual ~GameLayer() = default;

  private:
    auto OnAdd() -> bool override;

    auto OnUpdate() -> void override;

    auto OnRemove() -> void override;

    auto RenderImGui() -> void override;

    auto AddInputBindings() -> void;

    auto RenderMainMenu() -> void;

    Hzn::BufferLayout colorVertexBufferLayout = {{Hzn::Type::Vec3, "aPosition"}, {Hzn::Type::Vec3, "aColor"}};
    Hzn::BufferLayout textureVertexBufferLayout = {{Hzn::Type::Vec3, "aPosition"}, {Hzn::Type::Vec2, "aTexCoord"}};

    std::vector<ColorVertex> squareVertices = {{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}},
                                               {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                                               {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                                               {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}}};
    std::vector<Face> squareIndices = {{0, 1, 3}, {0, 2, 3}};

    std::vector<ColorVertex> cubeVertices = {
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}},  // front
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},   // front
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},   // front
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},    // front
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // back
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},  // back
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},  // back
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},   // back
    };

    std::vector<Face> cubeIndices = {{0, 1, 3}, {0, 2, 3}, {4, 5, 7}, {4, 6, 7}};

    std::optional<Hzn::CameraController<Hzn::PerspectiveCamera>> cameraController;
    std::shared_ptr<Hzn::VertexArray> mVertexArray;
    std::shared_ptr<Hzn::Shader> mShader;

    float mAspectRatio{0.0f};
    bool mRightMouseButtonPressed{false};
    bool mShowDemoWindow{false};

    std::unordered_set<std::filesystem::path> mTextureFilePaths;
    std::unordered_set<std::filesystem::path> mShaderFilePaths;

    std::optional<EditorUI> mEditorUI{std::nullopt};
};

} // namespace HznEditor

#endif // HORIZON_EDITOR_GAME_LAYER_H