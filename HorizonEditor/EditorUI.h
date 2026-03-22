#ifndef HORIZON_EDITOR_EDITOR_UI_H
#define HORIZON_EDITOR_EDITOR_UI_H

#include "EditorMenu.h"
#include "Variants.h"

#include <algorithm>
#include <vector>

#include <spdlog/spdlog.h>

namespace HznEditor
{

class EditorUI
{
  public:
    explicit EditorUI(Hzn::Layer &layer) : mLayer(layer)
    {
    }

    ~EditorUI() = default;

    void Render();

    void CreateMainMenuItem(std::string_view const &menuLabel)
    {
        if (std::find_if(mMainMenus.begin(), mMainMenus.end(),
                         [&](auto const &mainMenu) { return mainMenu.Name() == menuLabel; }) != mMainMenus.end())
        {
            SPDLOG_WARN("Main menu item already exists. Name: {}", menuLabel);
            return;
        }
        mMainMenus.emplace_back(mLayer, menuLabel);
    }

    template <UIToolConcept ToolType> void CreateUITool(std::string_view const &toolName, std::string_view const &menuLabel)
    {
        auto menuIter = std::find_if(mMainMenus.begin(), mMainMenus.end(),
                                     [&menuLabel](auto const &mainMenu) { return mainMenu.Name() == menuLabel; });
        if (menuIter == mMainMenus.end())
        {
            SPDLOG_WARN("Main menu item not found. Name: {}", menuLabel);
            return;
        }

        mUITools.emplace_back(
            std::make_shared<UIToolVariant>(std::in_place_type<ToolType>, mLayer, *menuIter, toolName));
        menuIter->AddUIToolReference(mUITools.back());
    }

  private:
    void RenderMainMenuBar();
    void RenderActiveTools();

    Hzn::Layer &mLayer;
    std::vector<EditorMenu> mMainMenus;
    std::vector<std::shared_ptr<UIToolVariant>> mUITools;
};

} // namespace HznEditor

#endif // HORIZON_EDITOR_EDITOR_UI_H
