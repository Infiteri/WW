#include "PostProcessor.h"
#include "ShaderSystem.h"

namespace WW
{
    void PostProcessor::Add(const std::string &shaderPath)
    {
        if (effects.find(shaderPath) != effects.end())
            return;

        PostProcessEffect effect;
        effect.Shader = ShaderSystem::LoadShader(shaderPath);
        effect.Enabled = true;
        effects[shaderPath] = effect;
    }

    void PostProcessor::Remove(const std::string &shaderPath)
    {
        effects.erase(shaderPath);
    }

    void PostProcessor::Enable(const std::string &shaderPath)
    {
        if (effects.find(shaderPath) == effects.end())
            return;

        effects[shaderPath].Enabled = true;
    }

    void PostProcessor::Disable(const std::string &shaderPath)
    {

        if (effects.find(shaderPath) == effects.end())
            return;

        effects[shaderPath].Enabled = false;
    }
}