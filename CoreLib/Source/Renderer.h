#pragma once

#include "Buffer/Framebuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader.h"
#include "Math/Transform.h"
#include "PostProcessor.h"
#include "Scene/Scene.h"
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
        static void BeginFrame();
        static void Render();
        static void EndFrame();
        static void Shutdown();

        static void ActivateScene(std::shared_ptr<Scene> scene);

        static void RenderCube(const Transform &transform, const Material &material);

        static void ActiavatePostProcessShader(const std::string &shaderName);
        static void DeactivatePostProcessShader(const std::string &shaderName);

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
            std::shared_ptr<Scene> ActiveScene;
        };

    private:
        static void InitializeGPUScreen();
        static void BeginGPUScreenFrame();
        static void EndGPUScreenFrame();
        static void ResizeGPUScreen();
    };
}
