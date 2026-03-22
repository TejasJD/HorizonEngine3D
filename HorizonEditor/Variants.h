#ifndef HZN_EDITOR_VARIANT_H
#define HZN_EDITOR_VARIANT_H

#include "AssetUITool.h"

#include <variant>

namespace HznEditor
{

using UIToolVariant = std::variant<AssetUITool>;

}

#endif // HZN_EDITOR_VARIANT_H