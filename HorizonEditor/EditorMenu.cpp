#include "EditorMenu.h"

#include <HorizonEngine/Layer.h>

#include <imgui.h>

#include <cassert>

namespace HznEditor
{

void EditorMenu::Render() const
{
    if (ImGui::BeginMenu(mName.data()))
    {
        std::for_each(mUITools.begin(), mUITools.end(), [](auto const &toolVariant) {
            assert(!toolVariant.expired());
            std::visit(
                [](auto &tool) {
                    if (ImGui::MenuItem(tool.Name().data()))
                    {
                        // TODO: possibly set window focus flag as well
                        tool.Show() = true;
                    }
                },
                *toolVariant.lock());
        });

        ImGui::EndMenu();
    }
}

} // namespace HznEditor