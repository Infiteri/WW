#pragma once

#include "Math/Vector.h"

namespace WW
{
    class Matrix4
    {
    public:
        union
        {
            float data[16];
            float m[4][4];
        };

        Matrix4();
        Matrix4(float *data);
        Matrix4(const Matrix4 &m);

        float *operator()() { return data; };
        Matrix4 operator*(const Matrix4 &o) { return Matrix4::Multiply(*this, o); }
        const float *operator[](int row) const { return m[row]; }

        static Matrix4 Perspective(float fov, float aspect, float near, float far);
        static Matrix4 Multiply(const Matrix4 &a, const Matrix4 &b);
        static Matrix4 Invert(const Matrix4 &a);

        static Matrix4 Translate(const Vector3 &position);
        static Matrix4 Scale(const Vector3 &scale);

        static Matrix4 RotateX(float x);
        static Matrix4 RotateY(float x);
        static Matrix4 RotateZ(float x);

        static Matrix4 RotateXYZ(const Vector3 &vec);
        static Matrix4 RotateZYX(const Vector3 &vec);
    };
} // namespace Core