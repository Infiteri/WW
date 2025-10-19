#include "Vector.h"

namespace WW
{
    Vector2::Vector2() { x = y = 0; }

    Vector2::Vector2(const Vector2 &other)
    {
        x = other.x;
        y = other.y;
    }

    Vector2::Vector2(float x, float y) : x(x), y(y) {}

    void Vector2::Set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Vector3::Vector3() { x = y = z = 0; }

    Vector3::Vector3(const Vector3 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    void Vector3::Set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
} // namespace Core