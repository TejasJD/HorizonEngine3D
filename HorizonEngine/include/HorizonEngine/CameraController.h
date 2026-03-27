#ifndef HORIZON_ENGINE_CAMERA_CONTROLLER_H
#define HORIZON_ENGINE_CAMERA_CONTROLLER_H

#include "HorizonEngine/Camera.h"

#include <type_traits>

namespace Hzn
{

template <typename Camera>
concept IsCamera =
    requires { requires std::same_as<Camera, OrthographicCamera> || std::same_as<Camera, PerspectiveCamera>; };

template <typename Camera>
inline constexpr bool IsPerspectiveCamera = std::same_as<std::remove_cvref_t<Camera>, PerspectiveCamera>;

template <typename Camera>
inline constexpr bool IsOrthographicCamera = std::same_as<std::remove_cvref_t<Camera>, OrthographicCamera>;

template <typename Camera>
    requires IsCamera<Camera>
class CameraController
{
  public:
    CameraController(Camera const &camera, float const translationSpeed, float const rotationSpeed)
        : mCamera(camera), mTranslationSpeed(translationSpeed), mRotationSpeed(rotationSpeed)
    {
    }

    auto GetCamera() -> Camera &
    {
        return mCamera;
    }

    auto GetTranslationSpeed() -> float
    {
        return mTranslationSpeed;
    }

    auto SetTranslationSpeed(float const speed)
    {
        mTranslationSpeed = speed;
    }

    auto GetRotationSpeed() -> float
    {
        return mRotationSpeed;
    }

    auto SetRotationSpeed(float const speed)
    {
        mRotationSpeed = speed;
    }

    auto GetEulerAngles() -> glm::vec3 const &
    {
        return mEulerAngle;
    }

    auto MoveUp(float deltaTime) -> void
    {
        float const speed = mTranslationSpeed * deltaTime;
        glm::vec3 newPosition = mCamera.Position() + glm::vec3{0.0f, speed, 0.0f};
        mCamera.SetPosition(newPosition);
    }

    auto MoveDown(float deltaTime) -> void
    {
        float const speed = mTranslationSpeed * deltaTime;
        glm::vec3 newPosition = mCamera.Position() - glm::vec3{0.0f, speed, 0.0f};
        mCamera.SetPosition(newPosition);
    }

    auto MoveLeft(float deltaTime) -> void
    {
        if constexpr (IsOrthographicCamera<Camera>)
        {
            float const speed = mTranslationSpeed * deltaTime;
            glm::vec3 newPosition = mCamera.Position() - glm::vec3{speed, 0.0f, 0.0f};
            mCamera.SetPosition(newPosition);
        }

        if constexpr (IsPerspectiveCamera<Camera>)
        {
            float const speed = mTranslationSpeed * deltaTime;
            glm::vec3 const direction =
                speed * glm::normalize(glm::cross(glm::vec3{0.0f, 1.0f, 0.0f}, mCamera.Direction()));
            glm::vec3 newPosition = mCamera.Position() + direction;
            mCamera.SetPosition(newPosition);
        }
    }

    auto MoveRight(float deltaTime) -> void
    {
        if constexpr (IsOrthographicCamera<Camera>)
        {
            float const speed = mTranslationSpeed * deltaTime;
            glm::vec3 newPosition = mCamera.Position() + glm::vec3{speed, 0.0f, 0.0f};
            mCamera.SetPosition(newPosition);
        }

        if constexpr (IsPerspectiveCamera<Camera>)
        {
            float const speed = mTranslationSpeed * deltaTime;
            glm::vec3 const direction =
                speed * glm::normalize(glm::cross(mCamera.Direction(), glm::vec3{0.0f, 1.0f, 0.0f}));
            glm::vec3 newPosition = mCamera.Position() + direction;
            mCamera.SetPosition(newPosition);
        }
    }

    auto MoveForward(float deltaTime) -> void
        requires IsPerspectiveCamera<Camera>
    {
        auto newPosition = mCamera.Position() + (mTranslationSpeed * deltaTime * glm::normalize(mCamera.Direction()));
        mCamera.SetPosition(newPosition);
    }

    auto MoveBack(float deltaTime) -> void
        requires IsPerspectiveCamera<Camera>
    {
        auto newPosition = mCamera.Position() - (mTranslationSpeed * deltaTime * glm::normalize(mCamera.Direction()));
        mCamera.SetPosition(newPosition);
    }

    auto LookUp(float deltaTime, float deltaMovement) -> void
        requires IsPerspectiveCamera<Camera>
    {
        float const speed = deltaMovement * deltaTime * mRotationSpeed;
        if (mEulerAngle.x + speed < 89.0f)
        {
            mEulerAngle.x += speed;
            mCamera.SetDirection(Rotate());
        }
    }

    auto LookDown(float deltaTime, float deltaMovement) -> void
        requires IsPerspectiveCamera<Camera>
    {
        float const speed = deltaMovement * deltaTime * mRotationSpeed;
        if (mEulerAngle.x - speed > -89.0f)
        {
            mEulerAngle.x -= speed;
            auto direction = Rotate();
            mCamera.SetDirection(Rotate());
        }
    }

    auto LookLeft(float deltaTime, float deltaMovement) -> void
        requires IsPerspectiveCamera<Camera>
    {
        float const speed = deltaMovement * deltaTime * mRotationSpeed;
        if (mEulerAngle.y + speed > 180.0f)
        {
            mEulerAngle.y = -180.0f;
        }
        mEulerAngle.y += speed;
        mCamera.SetDirection(Rotate());
    }

    auto LookRight(float deltaTime, float deltaMovement) -> void
        requires IsPerspectiveCamera<Camera>
    {
        float const speed = deltaMovement * deltaTime * mRotationSpeed;
        if (mEulerAngle.y - speed < -180.0f)
        {
            mEulerAngle.y = 180.0f;
        }
        mEulerAngle.y -= speed;
        mCamera.SetDirection(Rotate());
    }

  private:
    auto Rotate() -> glm::vec3
    {
        return glm::vec3{glm::rotate(glm::mat4(1.0f), glm::radians(mEulerAngle.z), glm::vec3{0.0f, 0.0f, 1.0f}) *
                         glm::rotate(glm::mat4(1.0f), glm::radians(mEulerAngle.y), glm::vec3{0.0f, 1.0f, 0.0f}) *
                         glm::rotate(glm::mat4(1.0f), glm::radians(mEulerAngle.x), glm::vec3{1.0f, 0.0f, 0.0f}) *
                         glm::vec4{0.0f, 0.0f, -1.0f, 1.0f}};
    }

    Camera mCamera;
    glm::vec3 mEulerAngle{0.0f, 0.0f, 0.0f};
    float mTranslationSpeed;
    float mRotationSpeed;
};

} // namespace Hzn

#endif // HORIZON_ENGINE_CAMERA_CONTROLLER_H
