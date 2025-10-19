#include "Quaternion.h"
#include <cfloat>
#include <cmath>

namespace WW
{
    Quaternion::Quaternion(const Vector3 &axis, float angle)
    {
        float half = 0.5f * angle;
        float s = sin(half);
        float c = cos(half);

        x = s * axis.x;
        y = s * axis.y;
        z = s * axis.z;
        w = c;
    }

    Quaternion::Quaternion(float x, float y, float z, float w) { Set(x, y, z, w); }

    Quaternion::Quaternion(const Quaternion &other) { Set(other); }

    void Quaternion::Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void Quaternion::Set(const Quaternion &other) { Set(other.x, other.y, other.z, other.w); }

    void Quaternion::Normalize()
    {
        float d = w * w + x * x + y * y + z * z;
        if (d < FLT_EPSILON)
        {
            Set(1, 0, 0, 0);
            return;
        }

        d = 1.0f / sqrt(d);
        w *= d;
        x *= d;
        y *= d;
        z *= d;
    }

    Matrix4 Quaternion::GetMatrix() const
    {
        Quaternion n;
        n.Set(*this);
        n.Normalize();

        Matrix4 m;
        m.data[0] = 1 - 2 * (n.y * n.y + n.z * n.z);
        m.data[1] = 2 * (n.x * n.y - n.z * n.w);
        m.data[2] = 2 * (n.x * n.z + n.y * n.w);
        m.data[3] = 0;

        m.data[4] = 2 * (n.x * n.y + n.z * n.w);
        m.data[5] = 1 - 2 * (n.x * n.x + n.z * n.z);
        m.data[6] = 2 * (n.y * n.z - n.x * n.w);
        m.data[7] = 0;

        m.data[8] = 2 * (n.x * n.z - n.y * n.w);
        m.data[9] = 2 * (n.y * n.z + n.x * n.w);
        m.data[10] = 1 - 2 * (n.x * n.x + n.y * n.y);
        m.data[11] = 0;

        m.data[12] = 0;
        m.data[13] = 0;
        m.data[14] = 0;
        m.data[15] = 1;

        return m;
    }
} // namespace Core