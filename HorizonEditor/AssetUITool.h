#ifndef HZN_EDITOR_ASSET_UI_TOOL_H
#define HZN_EDITOR_ASSET_UI_TOOL_H

#include "EditorUITool.h"

namespace HznEditor
{

class AssetUITool : public UIToolBase
{
  public:
    AssetUITool(Hzn::Layer &layer, EditorMenu const &owningMenuItem, std::string_view const &name)
        : UIToolBase(layer, owningMenuItem, name)
    {
    }

    void Render();

  private:
};

static_assert(UIToolConcept<AssetUITool>);

} // namespace HznEditor

#endif // HZN_EDITOR_ASSET_UI_TOOL_H