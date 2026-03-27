#include "EditorCameraTool.h"

#include <imgui.h>

namespace HznEditor
{

void EditorCameraTool::RenderCameraTransform()
{
    if (ImGui::BeginTable("CameraTransform", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp))
    {
        for (auto &&data : {mController.GetCamera().Position(), mController.GetEulerAngles()})
        {
            ImGui::TableNextRow();
            for (size_t i = 0; i < data.length(); ++i)
            {
                ImGui::TableNextColumn();
                ImGui::Text("%.2f", data[i]);
            }
        }
        ImGui::EndTable();
    }
}

void EditorCameraTool::UpdateCurrentSettings()
{
    mCurrentSettings.Position = mController.GetCamera().Position();
    mCurrentSettings.Direction = mController.GetCamera().Direction();
    mCurrentSettings.TranslationSpeed = mController.GetTranslationSpeed();
    mCurrentSettings.RotationSpeed = mController.GetRotationSpeed();
}

void EditorCameraTool::Render()
{
    UpdateCurrentSettings();

    if (!mShow)
    {
        return;
    }

    ImGui::Begin(mName.data(), &mShow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SeparatorText("Transform");
    RenderCameraTransform();

    ImGui::SeparatorText("Settings");
    ImGui::SliderFloat("Translation speed", &mCurrentSettings.TranslationSpeed, 1.0f, 10.0f);
    ImGui::SliderFloat("Rotation speed", &mCurrentSettings.RotationSpeed, 10.0f, 100.0f);

    if (ImGui::Button("Reset position and direction"))
    {
        mCurrentSettings.Position = mBaseSettings.Position;
        mCurrentSettings.Direction = mBaseSettings.Direction;

        mController.GetCamera().SetPosition(mCurrentSettings.Position);
        mController.GetCamera().SetDirection(mCurrentSettings.Direction);
    }

    if (ImGui::Button("Reset translation and rotation speed"))
    {
        mCurrentSettings.TranslationSpeed = mBaseSettings.TranslationSpeed;
        mCurrentSettings.RotationSpeed = mBaseSettings.RotationSpeed;

        mController.SetRotationSpeed(mCurrentSettings.RotationSpeed);
        mController.SetTranslationSpeed(mCurrentSettings.TranslationSpeed);
    }

    ImGui::End();
}

} // namespace HznEditor