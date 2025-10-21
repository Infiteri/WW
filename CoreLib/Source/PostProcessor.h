#pragma once

#include "Shader.h"
#include <memory>
#include <string>
#include <unordered_map>

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

    public:
        struct PostProcessEffect
        {
            std::shared_ptr<Shader> Shader;
            bool Enabled = false;
        };

    private:
        std::unordered_map<std::string, PostProcessEffect>
            effects;

    public:
        inline std::unordered_map<std::string, struct PostProcessEffect> &GetEffects()
        {
            return effects;
        }
    };
}
