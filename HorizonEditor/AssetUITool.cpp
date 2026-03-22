#include "AssetUITool.h"

#include <imgui.h>

namespace HznEditor
{

void AssetUITool::Render()
{
    if (!mShow)
    {
        return;
    }

    ImGui::Begin(mName.data(), &mShow);
    ImGui::Text("Assets window");
    ImGui::End();
}

} // namespace HznEditor