#ifndef HORIZON_EDITOR_H
#define HORIZON_EDITOR_H

#include "HorizonEngine/App.h"
#include "HorizonEngine/Input.h"

#include <glm/glm.hpp>

namespace HznEditor
{

class EditorApp : public Hzn::App
{
  public:
    auto Initialize() -> bool override;
    virtual ~EditorApp();
};

} // namespace HznEditor

#endif // HORIZON_EDITOR_H