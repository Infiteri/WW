#include "PostProcessor.h"
#include "ShaderSystem.h"
#include <algorithm>

namespace WW
{
    void PostProcessor::Add(const std::string &shaderPath)
    {
        // Prevent duplicates
        auto it = std::find_if(effects.begin(), effects.end(),
            [&](const PostProcessEffect &e) { return e.Path == shaderPath; });
        if (it != effects.end())
            return;

        PostProcessEffect effect;
        effect.Path = shaderPath;
        effect.Shader = ShaderSystem::LoadShader(shaderPath);
        effect.Enabled = true;
        effects.push_back(effect);
    }

    void PostProcessor::Remove(const std::string &shaderPath)
    {
        effects.erase(std::remove_if(effects.begin(), effects.end(),
            [&](const PostProcessEffect &e) { return e.Path == shaderPath; }),
            effects.end());
    }

    void PostProcessor::Enable(const std::string &shaderPath)
    {
        for (auto &e : effects)
            if (e.Path == shaderPath)
                e.Enabled = true;
    }

    void PostProcessor::Disable(const std::string &shaderPath)
    {
        for (auto &e : effects)
            if (e.Path == shaderPath)
                e.Enabled = false;
    }

    void PostProcessor::EnableAndAdd(const std::string &shaderPath)
    {
        auto it = std::find_if(effects.begin(), effects.end(),
            [&](const PostProcessEffect &e) { return e.Path == shaderPath; });
        if (it == effects.end())
            Add(shaderPath);
        else
            it->Enabled = true;
    }

    void PostProcessor::DisableAndAdd(const std::string &shaderPath)
    {
        auto it = std::find_if(effects.begin(), effects.end(),
            [&](const PostProcessEffect &e) { return e.Path == shaderPath; });
        if (it == effects.end())
            Add(shaderPath);
        else
            it->Enabled = false;
    }
}
