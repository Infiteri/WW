#include "Shader.h"
#include "Debug/WWDebug.h"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

#include <Windows.h>

#define ERR(msg, ...) WW_LOG_LVL("SHADER ERROR", msg, ##__VA_ARGS__)

namespace WW
{
    Shader::Shader() : id(0) {}

    Shader::Shader(const char *filePath) : id(0)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            ERR("Failed to open shader file: %s", filePath);
            return;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string src = ss.str();

        std::string vertSrc, fragSrc;
        size_t vertPos = src.find("// VERTEX");
        size_t fragPos = src.find("// FRAGMENT");

        if (vertPos == std::string::npos || fragPos == std::string::npos)
        {
            ERR("Shader file missing // VERTEX or // FRAGMENT markers!", "");
            return;
        }

        vertSrc = src.substr(vertPos + 9, fragPos - (vertPos + 9)); // 9 = length of "// VERTEX"
        fragSrc = src.substr(fragPos + 11);                         // 11 = length of "// FRAGMENT"

        Compile(vertSrc.c_str(), fragSrc.c_str());
    }

    Shader::Shader(const char *vertSrc, const char *fragSrc) : id(0) { Compile(vertSrc, fragSrc); }

    Shader::~Shader()
    {
        uniforms.clear();
        if (id)
            glDeleteProgram(id);
    }

    void Shader::Use()
    {
        if (id)
            glUseProgram(id);
    }

    void Shader::Mat4(float *data, const char *name)
    {
        uint32_t id = GetUniId(name);
        glUniformMatrix4fv(id, 1, GL_FALSE, data);
    }

    void Shader::Mat4(const Matrix4 &mat4, const char *name) { Mat4((float *)mat4.data, name); }

    void Shader::Int(int data, const char *name)
    {
        uint32_t id = GetUniId(name);
        glUniform1i(id, data);
    }

    void Shader::Float(float data, const char *name)
    {
        uint32_t id = GetUniId(name);
        glUniform1f(id, data);
    }

    void Shader::Vec2(const Vector2 &vec, const char *name)
    {
        uint32_t id = GetUniId(name);
        glUniform2f(id, vec.x, vec.y);
    }

    void Shader::Vec3(const Vector3 &vec, const char *name)
    {
        uint32_t id = GetUniId(name);
        glUniform3f(id, vec.x, vec.y, vec.z);
    }

    void Shader::Color4(const Color &color, const char *name)
    {
        uint32_t id = GetUniId(name);
        Color c = color.Normalized();
        glUniform4f(id, c.r, c.g, c.b, c.a);
    }

    void Shader::Compile(const char *vertSrc, const char *fragSrc)
    {
        uint32_t vert = CompileShader(GL_VERTEX_SHADER, vertSrc);
        uint32_t frag = CompileShader(GL_FRAGMENT_SHADER, fragSrc);

        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            ERR("Unable to link program: \n %s", infoLog);
        }

        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    uint32_t Shader::CompileShader(uint32_t type, const char *src)
    {
        uint32_t shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            ERR("Unable to compile shader: \n %s", infoLog);
        }

        return shader;
    }

    uint32_t Shader::GetUniId(const char *name)
    {
        auto it = uniforms.find(name);
        if (it != uniforms.end())
            return it->second;

        int location = glGetUniformLocation(id, name);
        if (location == -1)
            ERR("Warning: uniform '%s' not found!", name);

        uniforms[name] = location;
        return location;
    }
} // namespace Core