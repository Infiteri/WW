#include "PerspectiveCamera.h"
#include "Math/Math.h"

namespace WW
{
    PerspectiveCamera::PerspectiveCamera()
    {
        UpdateProjection(
            90, 16.0f / 9.0f, 0.001,
            1000.0f);
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    {
        UpdateProjection(fov, aspect, near, far);
    }

    void PerspectiveCamera::UpdateProjection()
    {
        projection = Matrix4::Perspective(fov * DEG_TO_RAD, aspect, near, far);
    }

    void PerspectiveCamera::UpdateProjection(float aspect)
    {
        this->aspect = aspect;
        UpdateProjection();
    }

    void PerspectiveCamera::UpdateProjection(float fov, float aspect, float near, float far)
    {
        this->fov = fov;
        this->near = near;
        this->far = far;
        UpdateProjection(aspect);
    }

    void PerspectiveCamera::SetFOV(const float fov, bool recalculateProjection)
    {
        if (this->fov != fov)
        {
            this->fov = fov;
            if (recalculateProjection)
                UpdateProjection();
        }
    }

    void PerspectiveCamera::SetAspect(const float aspect, bool recalculateProjection)
    {
        if (this->aspect != aspect)
        {
            this->aspect = aspect;
            if (recalculateProjection)
                UpdateProjection();
        }
    }

    void PerspectiveCamera::SetNear(const float near, bool recalculateProjection)
    {
        if (this->near != near)
        {
            this->near = near;
            if (recalculateProjection)
                UpdateProjection();
        }
    }

    void PerspectiveCamera::SetFar(const float far, bool recalculateProjection)
    {
        if (this->far != far)
        {
            this->far = far;
            if (recalculateProjection)
                UpdateProjection();
        }
    }

} // namespace Core