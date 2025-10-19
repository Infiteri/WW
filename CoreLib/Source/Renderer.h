#pragma once

#include "Buffer/Framebuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader.h"
#include <memory>

namespace WW
{
    class Renderer
    {
    public:
        static void Viewport(float w, float h);

        static void Init();
        static void Render();
        static void Shutdown();

    private:
        struct GPUScreen
        {
            std::unique_ptr<VertexArray> Array;
            std::unique_ptr<Framebuffer> Buffer;
            std::shared_ptr<Shader> ScreenShader;
        };

        static void InitializeGPUScreen();
        static void BeginGPUScreenFrame();
        static void EndGPUScreenFrame();
        static void ResizeGPUScreen();

    public:
        struct Viewport
        {
            float Width;
            float Height;
            inline float GetAspect() const { return Width / Height; };
        };

        struct State
        {
            struct Viewport Viewport;
            GPUScreen Screen;
        };
    };
}