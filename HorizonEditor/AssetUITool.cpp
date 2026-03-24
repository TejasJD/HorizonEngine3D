#include "AssetUITool.h"

#include <HorizonEngine/Layer.h>
#include <HorizonEngine/Platform.h>

#include <imgui.h>

namespace HznEditor
{

static constexpr ImGuiComboFlags COMBO_FLAGS = ImGuiComboFlags_NoPreview;

void AssetUITool::Render()
{
    if (!mShow)
    {
        return;
    }

    ImGui::Begin(mName.data(), &mShow, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginTable("OptionsButtonGrid", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Button("Load shader"))
        {
        }

        ImGui::TableNextColumn();
        if (ImGui::Button("Load texture"))
        {
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Button("Clear all shaders"))
        {
        }

        ImGui::TableNextColumn();
        if (ImGui::Button("Clear all textures"))
        {
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::BeginCombo("Loaded Shaders", nullptr, COMBO_FLAGS))
        {
            ImGui::EndCombo();
        }

        ImGui::TableNextColumn();
        if (ImGui::BeginCombo("Loaded textures", nullptr, COMBO_FLAGS))
        {
            ImGui::EndCombo();
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace HznEditor