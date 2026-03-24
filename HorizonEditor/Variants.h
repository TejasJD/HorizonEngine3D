#ifndef HZN_EDITOR_VARIANT_H
#define HZN_EDITOR_VARIANT_H

#include "AssetUITool.h"
#include "ImGuiDemoWindowTool.h"

#include <variant>

namespace HznEditor
{

using UIToolVariant = std::variant<AssetUITool, ImGuiDemoWindowTool>;

}

#endif // HZN_EDITOR_VARIANT_H