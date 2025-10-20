#pragma once

#include "Shader.h"
#include <memory>
#include <unordered_map>

namespace WW
{
    class ShaderSystem
    {
    public:
        static void Init();
        static void Shutdown();

        static std::shared_ptr<Shader> GetShader(const std::string &name);
        static std::shared_ptr<Shader> LoadShader(const std::string &name);

    public:
        struct State
        {
            std::unordered_map<std::string, std::shared_ptr<Shader>> Shaders;
        };
    };
}