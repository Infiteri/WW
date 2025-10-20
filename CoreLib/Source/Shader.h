#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Color.h"
#include <string>
#include <stdint.h>
#include <unordered_map>

namespace WW
{
    class Shader
    {
    public:
        Shader();
        Shader(const char *filePath);
        Shader(const char *vertSrc, const char *fragSource);
        ~Shader();

        void Use();

        void Mat4(float *data, const char *name);
        void Mat4(const Matrix4 &mat4, const char *name);
        void Int(int data, const char *name);
        void Float(float data, const char *name);
        void Vec2(const Vector2 &vec, const char *name);
        void Vec3(const Vector3 &vec, const char *name);
        void Color4(const Color &color, const char *name);

        inline uint32_t GetID() const { return id; };

    private:
        uint32_t id;
        std::unordered_map<std::string, uint32_t> uniforms;

    private:
        void Compile(const char *vertSrc, const char *fragSource);
        uint32_t CompileShader(uint32_t type, const char *src);

        uint32_t GetUniId(const char *name);
    };
} // namespace Core