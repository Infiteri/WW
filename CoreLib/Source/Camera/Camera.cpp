#include "Camera.h"
#include "Math/Vector.h"

namespace WW
{
    void Camera::SetPosition(const Vector3 &pos) { position = pos; }
    void Camera::SetRotation(const Vector3 &pos) { rotation = pos; }

    void Camera::UpdateView() { view = Matrix4::Translate(position) * Matrix4::RotateZYX(rotation); }
} // namespace Core