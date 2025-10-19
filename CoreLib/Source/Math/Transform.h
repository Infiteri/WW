#pragma once

#include "Math/Vector.h"
#include "Matrix.h"

namespace WW
{
    struct Transform
    {
        Vector3 Position;
        Vector3 Rotation;
        Vector3 Scale{1, 1, 1};

        Vector3 &operator[](int index)
        {
            switch (index)
            {
            case 0:
                return Position;
                break;
            case 1:
                return Rotation;
                break;
            default:
                return Scale;
                break;
            }
        }

        Matrix4 GetMatrix() const;
    };
} // namespace Core