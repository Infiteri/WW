#pragma once

namespace WW
{
    class Vector2
    {
    public:
        float x, y;

        Vector2();
        Vector2(const Vector2 &other);
        Vector2(float x, float y);
        ~Vector2() = default;

        Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }

        Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }

        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

        Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

        Vector2 operator*(const Vector2 &other) const { return Vector2(x * other.x, y * other.y); }

        bool operator==(const Vector2 &other) const { return x == other.x && y == other.y; }

        bool operator!=(const Vector2 &other) const { return !(*this == other); }

        void Set(float x, float y);
    };

    class Vector3
    {
    public:
        float x, y, z;

        Vector3();
        Vector3(const Vector3 &other);
        Vector3(float x, float y, float z);
        ~Vector3() = default;

        Vector3 operator+(const Vector3 &other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3 &other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3 operator/(float scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        Vector3 operator*(const Vector3 &other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        bool operator==(const Vector3 &other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Vector3 &other) const { return !(*this == other); }

        void Set(float x, float y, float z);
    };

} // namespace Core