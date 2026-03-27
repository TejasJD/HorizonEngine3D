#ifndef HZN_EDITOR_CAMERA_TOOL_H
#define HZN_EDITOR_CAMERA_TOOL_H

#include "EditorUITool.h"

#include <HorizonEngine/Camera.h>
#include <HorizonEngine/CameraController.h>

#include <glm/glm.hpp>

namespace HznEditor
{

struct CameraControllerSettings
{
    glm::vec3 Position;
    glm::vec3 Direction;
    float TranslationSpeed;
    float RotationSpeed;
    float Fov;
};

class EditorCameraTool : public UIToolBase
{
  public:
    EditorCameraTool(EditorMenu const &owningMenuItem, std::string_view const &name,
                     Hzn::CameraController<Hzn::PerspectiveCamera> &controller)
        : UIToolBase(owningMenuItem, name), mController(controller),
          mBaseSettings{mController.GetCamera().Position(), mController.GetCamera().Direction(),
                        mController.GetTranslationSpeed(), mController.GetRotationSpeed(),
                        mController.GetCamera().Fov()},
          mCurrentSettings(mBaseSettings)
    {
    }

    void Render();

  private:
    void RenderCameraTransform();

    void UpdateCurrentSettings();

    Hzn::CameraController<Hzn::PerspectiveCamera> &mController;
    CameraControllerSettings const mBaseSettings;
    CameraControllerSettings mCurrentSettings;
};

static_assert(UIToolConcept<EditorCameraTool>);

}; // namespace HznEditor

#endif // HZN_EDITOR_CAMERA_TOOL_H