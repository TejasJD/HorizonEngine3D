#ifndef HORIZON_EDITOR_H
#define HORIZON_EDITOR_H

#include "HorizonEngine/App.h"
#include "HorizonEngine/Input.h"

#include <glm/glm.hpp>

namespace Hzn
{

class HorizonEditor : public App
{
  public:
    auto Initialize() -> bool override;

    virtual ~HorizonEditor();
};

} // namespace Hzn

#endif // HORIZON_EDITOR_H