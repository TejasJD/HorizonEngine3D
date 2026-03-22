#include "EditorUI.h"

#include <imgui.h>

#include <algorithm>

namespace HznEditor
{

void EditorUI::Render()
{
    RenderMainMenuBar();

    RenderActiveTools();
}

void EditorUI::RenderMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        std::for_each(mMainMenus.begin(), mMainMenus.end(), [](auto const &item) { item.Render(); });
        ImGui::EndMainMenuBar();
    }
}

void EditorUI::RenderActiveTools()
{
    std::for_each(mUITools.begin(), mUITools.end(),
                  [](auto const &toolVariant) { std::visit([](auto &tool) { tool.Render(); }, *toolVariant); });
}

} // namespace HznEditor