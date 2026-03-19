#include "GameLayer.h"

#include "HorizonEngine/KeyCodes.h"
#include "HorizonEngine/MouseCodes.h"
#include "HorizonEngine/RendererAPI.h"

#include <algorithm>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace Hzn
{

auto GameLayer::OnAdd() -> bool
{
    SPDLOG_INFO("GameLayer::{}", __func__);
    mAspectRatio = AppRef().WindowRef().AspectRatio();
    cameraController = CameraController<PerspectiveCamera>{PerspectiveCamera{{0.0f, 0.0f, 1.0f}, mAspectRatio}, 1.0f};

    mVertexArray = VertexArray::Create();
    assert(!!mVertexArray);
    auto vertexBuffer = VertexBuffer::Create(reinterpret_cast<float const *>(cubeVertices.data()),
                                             cubeVertices.size() * sizeof(ColorVertex), colorVertexBufferLayout);

    auto elementBuffer = ElementBuffer::Create(reinterpret_cast<uint32_t const *>(cubeIndices.data()),
                                               cubeIndices.size() * sizeof(Face));
    assert(!!vertexBuffer && !!elementBuffer);

    mVertexArray->AddVertexBuffer(vertexBuffer);
    mVertexArray->SetElementBuffer(elementBuffer);

    mShader = Shader::Create({{ShaderType::Vertex, "resources/shaders/shader.vert"},
                              {ShaderType::Fragment, "resources/shaders/shader.frag"}});
    if (!mShader)
    {
        return false;
    }

    RendererAPI::Enable(EnableOptions::DebugOutput);
    RendererAPI::Enable(EnableOptions::DepthTest);

    AddInputBindings();

    return true;
}

auto GameLayer::OnUpdate() -> void
{
    RendererAPI::Clear(BufferBit::Depth);

    assert(cameraController.has_value());
    mAspectRatio = AppRef().WindowRef().AspectRatio();
    cameraController->GetCamera().SetAspectRatio(mAspectRatio);
    auto uPvm = cameraController->GetCamera().ProjectionViewMatrix() *
                glm::translate(glm::mat4(1.0f), glm::vec3{0.0f, 0.0f, -1.0f});

    mShader->SetUniform(ShaderType::Vertex, uPvm, "uPvm");
    RendererAPI::DrawElements(mVertexArray, mShader, cubeIndices.size() * 3);
}

auto GameLayer::RenderImGui() -> void
{
    static std::optional<std::filesystem::path> filePathOpt;

    ImGui::Begin("Textures");

    if (ImGui::Button("Add texture file path"))
    {
        auto filePathOpt = AppRef().PlatformRef().GetFilePathFromDialog();

        if (filePathOpt.has_value())
        {
            mTextureFilePaths.insert(filePathOpt.value());
        }
    }

    if (ImGui::Button("Clear all textures"))
    {
        mTextureFilePaths.clear();
    }

    std::for_each(mTextureFilePaths.begin(), mTextureFilePaths.end(),
                  [](auto const &filePath) { ImGui::Text("%s", filePath.string().c_str()); });

    ImGui::End();

    ImGui::Begin("Shader file path");

    if (ImGui::Button("Add shader file path"))
    {
        auto filePathOpt = AppRef().PlatformRef().GetFilePathFromDialog();

        if (filePathOpt.has_value())
        {
            mShaderFilePaths.insert(filePathOpt.value());
        }
    }

    if (ImGui::Button("Clear all textures"))
    {
        mShaderFilePaths.clear();
    }

    std::for_each(mShaderFilePaths.begin(), mShaderFilePaths.end(),
                  [](auto const &filePath) { ImGui::Text("%s", filePath.string().c_str()); });

    ImGui::End();
}

auto GameLayer::OnRemove() -> void
{
    SPDLOG_INFO("GameLayer::{}", __func__);
}

auto GameLayer::AddInputBindings() -> void
{
    SPDLOG_INFO("GameLayer::{}", __func__);
    auto &inputManager = AppRef().InputManagerRef();

    inputManager.SetKeyCallback(KeyCode::W, [this](ButtonState const state) {
        switch (state)
        {
        case ButtonState::Pressed: {
            assert(!!cameraController);
            cameraController->MoveForward(AppRef().DeltaTime());
            break;
        }
        case ButtonState::Released: {
            break;
        }
        default:
            assert(false && "Unknown button state");
            break;
        }
    });

    inputManager.SetKeyCallback(KeyCode::S, [this](ButtonState const state) {
        switch (state)
        {
        case ButtonState::Pressed: {
            assert(cameraController.has_value());
            cameraController->MoveBack(AppRef().DeltaTime());
            break;
        }
        case ButtonState::Released: {
            break;
        }
        default:
            assert(false && "Unknown button state");
            break;
        }
    });

    inputManager.SetKeyCallback(KeyCode::A, [this](ButtonState const state) {
        switch (state)
        {
        case ButtonState::Pressed: {
            assert(!!cameraController);
            cameraController->MoveLeft(AppRef().DeltaTime());
            break;
        }
        case ButtonState::Released: {
            break;
        }
        default:
            assert(false && "Unknown button state");
            break;
        }
    });

    inputManager.SetKeyCallback(KeyCode::D, [this](ButtonState const state) {
        switch (state)
        {
        case ButtonState::Pressed: {
            assert(cameraController.has_value());
            cameraController->MoveRight(AppRef().DeltaTime());
            break;
        }
        case ButtonState::Released: {
            break;
        }
        default:
            assert(false && "Unknown button state");
            break;
        }
    });

    inputManager.SetMouseButtonCallback(MouseCode::ButtonRight, [this](ButtonState const state) {
        switch (state)
        {
        case ButtonState::Pressed: {
            mRightMouseButtonPressed = true;
            break;
        }
        case ButtonState::Released: {
            mRightMouseButtonPressed = false;
            break;
        }
        default:
            assert(false && "Unknown button state");
            break;
        }
    });

    inputManager.SetScrollCallback([this](double const xOffset, double const yOffset) {
        assert(cameraController.has_value());
        float fov = cameraController->GetCamera().Fov();
        float delta = AppRef().DeltaTime() * (static_cast<float>(yOffset) * 10.0f);
        cameraController->GetCamera().SetFov(fov + delta);
    });

    inputManager.SetCursorCallback([this](double const xOffset, double const yOffset) {
        assert(cameraController.has_value());
        static float xPrev = xOffset;
        static float yPrev = yOffset;

        if (mRightMouseButtonPressed)
        {
            float xDir = xOffset - xPrev;
            float yDir = yOffset - yPrev;

            float moveInXDirBy = glm::abs(xDir);
            float moveInYDirBy = glm::abs(yDir);

            if (xDir > 0.0f)
            {
                cameraController->LookRight(AppRef().DeltaTime(), moveInXDirBy);
            }
            else
            {
                cameraController->LookLeft(AppRef().DeltaTime(), moveInXDirBy);
            }

            if (yDir > 0.0f)
            {
                cameraController->LookDown(AppRef().DeltaTime(), moveInYDirBy);
            }
            else
            {
                cameraController->LookUp(AppRef().DeltaTime(), moveInYDirBy);
            }
        }

        xPrev = xOffset;
        yPrev = yOffset;
    });
}

} // namespace Hzn