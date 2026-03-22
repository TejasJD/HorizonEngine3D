#ifndef HZN_EDITOR_EDITOR_MENU_ITEM_H
#define HZN_EDITOR_EDITOR_MENU_ITEM_H

#include "EditorUITool.h"
#include "Variants.h"

#include <HorizonEngine/Layer.h>

#include <spdlog/spdlog.h>

#include <cassert>

namespace HznEditor
{

class EditorMenu
{
  public:
    explicit EditorMenu(Hzn::Layer &layer, std::string_view const &name) : mName(name), mOwningLayer(layer)
    {
    }

    void AddUIToolReference(std::shared_ptr<UIToolVariant> const &tool)
    {
        assert(!!tool && "tool is NULL");
        mUITools.emplace_back(tool);
        SPDLOG_INFO("EditorMenu::{}, num UI tools in {}: {}", __func__, mName, mUITools.size());
    }

    std::string_view const &Name() const
    {
        return mName;
    }

    void Render() const;

  private:
    Hzn::Layer &mOwningLayer;
    std::string_view mName;
    std::vector<std::weak_ptr<UIToolVariant>> mUITools;
};

} // namespace HznEditor

#endif // HZN_EDITOR_EDITOR_MENU_ITEM_H