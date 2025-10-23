#pragma once

#include "Shader.h"
#include <memory>
#include <string>
#include <vector>

namespace WW
{
    class PostProcessor
    {
    public:
        PostProcessor() = default;
        ~PostProcessor() = default;

        void Add(const std::string &shaderPath);
        void Remove(const std::string &shaderPath);
        void Enable(const std::string &shaderPath);
        void Disable(const std::string &shaderPath);
        void EnableAndAdd(const std::string &shaderPath);
        void DisableAndAdd(const std::string &shaderPath);

        struct PostProcessEffect
        {
            std::string Path;
            std::shared_ptr<Shader> Shader;
            bool Enabled = false;
        };

        inline std::vector<PostProcessEffect> &GetEffects()
        {
            return effects;
        }

    private:
        std::vector<PostProcessEffect> effects;
    };
}
