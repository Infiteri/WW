#pragma once

#include <vector>
#include <stdint.h>

namespace WW
{

    /// @brief Render pass texture type
    enum class RPTextureType
    {
        Depth,
        Rgb,
        Rgba8
    };

    struct FBRenderPass
    {
        RPTextureType Type;
        uint32_t Index, Id;
    };

    struct FramebufferConfiguration
    {
        float Width, Height;

        std::vector<FBRenderPass> Passes;
    };

    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferConfiguration &cfg);
        ~Framebuffer();

        void Bind();
        void Unbind();
        void Create();
        void Resize(float w, float h);

        FBRenderPass *GetRenderPass(int index);

        inline uint32_t GetID() const { return id; };

    private:
        uint32_t id;
        FramebufferConfiguration state;

    private:
        void DestroyForResize();
    };
} // namespace Core
