#pragma once

#include "Buffer/Framebuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader.h"
#include "PostProcessor.h"
#include "BatchRenderer.h"
#include "Camera/PerspectiveCamera.h"
#include "BatchRenderer.h"
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
            std::unique_ptr<Framebuffer> PostBuffer;
            std::shared_ptr<Shader> Shader;
        };

    public:
        struct State
        {
            struct Viewport
            {
                float Width = 0.0f;
                float Height = 0.0f;
                inline float GetAspect() const { return Width / Height; }
            } Viewport;

            bool HasContext = false;

            PostProcessor Post;

            GPUScreen Screen;
            std::shared_ptr<Shader> MainShader;
            PerspectiveCamera Camera;
            BatchRenderer Batch;
        };

    private:
        static void InitializeGPUScreen();
        static void BeginGPUScreenFrame();
        static void EndGPUScreenFrame();
        static void ResizeGPUScreen();
    };
}
