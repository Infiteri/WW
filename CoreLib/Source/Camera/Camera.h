#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace WW
{
    class Camera
    {
    public:
        Camera() {};
        virtual ~Camera() {};

        void SetPosition(const Vector3 &pos);

        void UpdateView();
        virtual void UpdateProjection() {};

        inline void SetProjection(const Matrix4 &m) { projection = m; };

        inline const Matrix4 &GetProjection() const { return projection; };
        inline const Matrix4 &GetView() const { return view; };

        inline Vector3 &GetPosition() { return position; };

    protected:
        Matrix4 projection, view;
        Vector3 position;
    };
} // namespace Core