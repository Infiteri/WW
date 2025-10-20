#include "ShaderSystem.h"

namespace WW
{
    static ShaderSystem::State state;

    void ShaderSystem::Init()
    {
    }

    void ShaderSystem::Shutdown()
    {
    }

    std::shared_ptr<Shader> ShaderSystem::GetShader(const std::string &name)
    {
        if (state.Shaders.find(name) != state.Shaders.end())
        {
            return state.Shaders[name];
        }

        return LoadShader(name);
    }

    std::shared_ptr<Shader> ShaderSystem::LoadShader(const std::string &name)
    {
        if (state.Shaders.find(name) != state.Shaders.end())
        {
            return state.Shaders[name];
        }

        std::shared_ptr<Shader> shader = std::make_shared<Shader>(name.c_str());
        state.Shaders[name] = shader;
        return shader;
    }
}