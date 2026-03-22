#ifndef HZN_EDITOR_UI_TOOL_H
#define HZN_EDITOR_UI_TOOL_H

#include <HorizonEngine/Layer.h>

#include <string_view>

namespace HznEditor
{

struct EditorMenu;

template <typename T>
concept UIToolConcept = requires(T t, bool value) {
    { t.Render() } -> std::same_as<void>;
    { t.Name() } -> std::same_as<std::string_view const &>;
    { t.OwningMenuItem() } -> std::same_as<EditorMenu const &>;
    { t.Show() } -> std::same_as<bool &>;
};

class UIToolBase
{
  public:
    UIToolBase(Hzn::Layer &layer, EditorMenu const &owningMenuItem, std::string_view const &name)
        : mLayer(layer), mOwningMenuItem(owningMenuItem), mName(name)
    {
    }

    std::string_view const &Name() const
    {
        return mName;
    }

    EditorMenu const &OwningMenuItem() const
    {
        return mOwningMenuItem;
    }

    bool &Show()
    {
        return mShow;
    }

  protected:
    Hzn::Layer &mLayer;
    EditorMenu const &mOwningMenuItem;
    std::string_view mName;
    bool mShow{false};
};

} // namespace HznEditor

#endif // HZN_EDITOR_UI_TOOL_H