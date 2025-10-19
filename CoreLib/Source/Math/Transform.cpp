#include "Transform.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"

namespace WW
{

    Matrix4 Transform::GetMatrix() const
    {
        Quaternion quatX{{1, 0, 0}, Rotation.x * DEG_TO_RAD};
        Quaternion quatY{{0, 1, 0}, Rotation.y * DEG_TO_RAD};
        Quaternion quatZ{{0, 0, 1}, Rotation.z * DEG_TO_RAD};
        Quaternion quat = quatX * quatY * quatZ;

        Matrix4 translationMatrix = Matrix4::Translate(Position);
        Matrix4 scaleMatrix = Matrix4::Scale(Scale);
        Matrix4 rotationMatrix = quat.GetMatrix();

        return translationMatrix * rotationMatrix * scaleMatrix;
    }
} // namespace Core