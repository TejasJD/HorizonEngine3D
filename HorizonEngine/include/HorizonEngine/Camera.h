#ifndef HORIZON_ENGINE_CAMERA_H
#define HORIZON_ENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

namespace Hzn
{

class OrthographicCamera
{
  public:
    OrthographicCamera(glm::vec3 const &position, float const aspectRatio, float const zoom = 1.0f)
        : mPosition(position), mAspectRatio(aspectRatio), mZoom(zoom)
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
    }

    auto ViewMatrix() const -> glm::mat4 const &
    {
        return mViewMatrix;
    }

    auto ProjectionMatrix() const -> glm::mat4 const &
    {
        return mProjectionMatrix;
    }

    auto ProjectionViewMatrix() const -> glm::mat4
    {
        return mProjectionMatrix * mViewMatrix;
    }

    auto Position() const -> glm::vec3 const &
    {
        return mPosition;
    }

    auto AspectRatio() const -> float
    {
        return mAspectRatio;
    }

    auto Zoom() const -> float
    {
        return mZoom;
    }

    auto SetPosition(glm::vec3 const &position) -> void
    {
        if (glm::all(glm::epsilonEqual(position, mPosition, glm::epsilon<float>())))
        {
            return;
        }
        // SPDLOG_INFO("Position: [{}, {}, {}]", mPosition.x, mPosition.y, mPosition.z);

        mPosition = position;
        RecalculateViewMatrix();
    }

    auto SetAspectRatio(float const aspectRatio) -> void
    {
        if (glm::epsilonEqual(aspectRatio, mAspectRatio, glm::epsilon<float>()))
        {
            return;
        }
        mAspectRatio = aspectRatio;
        RecalculateProjectionMatrix();
    }

    auto SetZoom(float const zoom) -> void
    {
        if (glm::epsilonEqual(zoom, mZoom, glm::epsilon<float>()))
        {
            return;
        }
        mZoom = zoom;
        RecalculateProjectionMatrix();
    }

  private:
    auto RecalculateViewMatrix() -> void
    {
        auto lookingAt = mPosition + glm::vec3{0.0f, 0.0f, -1.0f};
        mViewMatrix = glm::lookAt(mPosition, lookingAt, glm::vec3{0.0f, 1.0f, 0.0f});
    }

    auto RecalculateProjectionMatrix() -> void
    {
        mProjectionMatrix = glm::ortho(-(mAspectRatio * mZoom), mAspectRatio * mZoom, -mZoom, mZoom, -100.0f, 100.0f);
    }

    glm::mat4 mViewMatrix{1.0f};
    glm::mat4 mProjectionMatrix{1.0f};
    glm::vec3 mPosition{0.0f};
    float mAspectRatio{0.0f};
    float mZoom{0.0f};
};

class PerspectiveCamera
{
  public:
    PerspectiveCamera(glm::vec3 position, float aspectRatio, float fovInDegrees = 45.0f)
        : mPosition(position), mAspectRatio(aspectRatio), mFov(fovInDegrees)
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
    }

    auto ViewMatrix() const -> glm::mat4 const &
    {
        return mViewMatrix;
    }

    auto ProjectionMatrix() const -> glm::mat4 const &
    {
        return mProjectionMatrix;
    }

    auto ProjectionViewMatrix() const -> glm::mat4
    {
        return mProjectionMatrix * mViewMatrix;
    }

    auto Position() const -> glm::vec3 const &
    {
        return mPosition;
    }

    auto Direction() const -> glm::vec3 const &
    {
        return mDirection;
    }

    auto AspectRatio() const -> float
    {
        return mAspectRatio;
    }

    auto Fov() const -> float
    {
        return mFov;
    }

    auto SetPosition(glm::vec3 const &position) -> void
    {
        if (glm::all(glm::epsilonEqual(mPosition, position, glm::epsilon<float>())))
        {
            return;
        }
        mPosition = position;
        RecalculateViewMatrix();
    }

    auto SetDirection(glm::vec3 const &direction) -> void
    {
        if (glm::all(glm::epsilonEqual(mDirection, direction, glm::epsilon<float>())))
        {
            return;
        }
        mDirection = direction;
        RecalculateViewMatrix();
    }

    auto SetAspectRatio(float const aspectRatio) -> void
    {
        if (glm::epsilonEqual(aspectRatio, mAspectRatio, glm::epsilon<float>()))
        {
            return;
        }
        mAspectRatio = aspectRatio;
        RecalculateProjectionMatrix();
    }

    auto SetFov(float const fovInDegrees) -> void
    {
        if (glm::epsilonEqual(mFov, fovInDegrees, glm::epsilon<float>()))
        {
            return;
        }
        mFov = fovInDegrees;
        RecalculateProjectionMatrix();
    }

  private:
    auto RecalculateViewMatrix() -> void
    {
        auto lookingAt = mPosition + mDirection;
        mViewMatrix = glm::lookAt(mPosition, lookingAt, glm::vec3{0.0f, 1.0f, 0.0f});
    }

    auto RecalculateProjectionMatrix() -> void
    {
        mProjectionMatrix = glm::perspective(mFov, mAspectRatio, 0.1f, 100.0f);
    }

    glm::mat4 mViewMatrix{1.0f};
    glm::mat4 mProjectionMatrix{1.0f};
    glm::vec3 mPosition{0.0f};
    glm::vec3 mDirection{0.0f, 0.0f, -1.0f};
    float mAspectRatio{0.0f};
    float mFov{0.0f};
};

} // namespace Hzn

#endif // HORIZON_ENGINE_CAMERA_H