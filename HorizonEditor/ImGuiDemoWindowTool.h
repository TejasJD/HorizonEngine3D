#ifndef HZN_EDITOR_IMGUI_DEMO_WINDOW_TOOL_H
#define HZN_EDITOR_IMGUI_DEMO_WINDOW_TOOL_H

#include "EditorUITool.h"

#include <imgui.h>

namespace HznEditor
{

class ImGuiDemoWindowTool : public UIToolBase
{
  public:
    ImGuiDemoWindowTool(EditorMenu const &owningMenuItem, std::string_view const &name)
        : UIToolBase(owningMenuItem, name)
    {
    }

    void Render()
    {
        if (mShow)
        {
            ImGui::ShowDemoWindow(&mShow);
        }
    }

  private:
};

static_assert(UIToolConcept<ImGuiDemoWindowTool>);

} // namespace HznEditor

#endif // HZN_EDITOR_IMGUI_DEMO_WINDOW_TOOL_H