#include "Renderer.h"
#include "Platform/Platform.h"
#include "Debug/WWDebug.h"
#include "Windows.h"
#include "ShaderSystem.h"
#include "BackgroundLayer.h"
#include "Math/Transform.h"
#include "Texture/Texture2D.h"
#include "Texture/TextureSystem.h"
#include <glad/glad.h>
#include <random>

namespace WW
{
    static Renderer::State state;
    void Renderer::Init()
    {
        state.HasContext = true;
        gladLoadGL();
        TextureSystem::Init();

        state.MainShader = ShaderSystem::LoadShader("Shader.glsl");

        InitializeGPUScreen();

        glEnable(GL_DEPTH_TEST);

        state.Camera.SetFOV(90.0f);
        state.Camera.SetPosition(Vector3(0.0f, 0.0f, -5.0f));
        state.Camera.UpdateView();

        state.Batch.Init();
    }

    void Renderer::BeginFrame()
    {
        if (!state.HasContext)
            return;
        BeginGPUScreenFrame();
        state.Camera.UpdateView();
    }

    void Renderer::Render()
    {
        if (!state.HasContext)
            return;

        Platform::UpdateTotalTime();

        state.Batch.Begin();
        state.MainShader->Use();

        if (state.ActiveScene)
            state.ActiveScene->Render();
    }

    void Renderer::EndFrame()
    {
        state.Batch.End(&state.Camera, state.MainShader.get());
        EndGPUScreenFrame();

        auto pass = state.Screen.Buffer->GetRenderPass(0);
        auto post = state.Screen.PostBuffer.get();

        post->Bind();
        state.Screen.Shader->Use();

        glActiveTexture(GL_TEXTURE0 + pass->Index);
        glBindTexture(GL_TEXTURE_2D, pass->Id);
        state.Screen.Shader->Int(pass->Index, "uScreenTexture");

        state.Screen.Array->Bind();
        state.Screen.Array->GetVertexBuffer()->Bind();
        state.Screen.Array->GetVertexBuffer()->Draw();

        // post processing goes here
        for (const auto &effect : state.Post.GetEffects())
        {
            if (!effect.Enabled)
                continue;

            effect.Shader->Use();
            effect.Shader->Float(Platform::GetTotalTime(), "uTime");
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, post->GetRenderPass(0)->Id);
            state.Screen.Array->Bind();
            state.Screen.Array->GetVertexBuffer()->Bind();
            state.Screen.Array->GetVertexBuffer()->Draw();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, post->GetRenderPass(0)->Id);

        state.Screen.Array->Bind();
        state.Screen.Array->GetVertexBuffer()->Bind();
        state.Screen.Array->GetVertexBuffer()->Draw();
    }

    void Renderer::Viewport(float w, float h)
    {
        state.Camera.SetAspect(w / h);
        state.Viewport.Width = w;
        state.Viewport.Height = h;
        if (!state.HasContext)
            return;
        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
        ResizeGPUScreen();
    }

    void Renderer::Shutdown()
    {
        if (!state.HasContext)
            return;
    }

    void Renderer::ActivateScene(std::shared_ptr<Scene> scene)
    {
        state.ActiveScene = scene;
    }

    void Renderer::RenderCube(const Transform &transform, const Material &material)
    {
        state.Batch.RenderCube(transform, material);
    }

    void Renderer::ActiavatePostProcessShader(const std::string &shaderName)
    {
        state.Post.EnableAndAdd(shaderName);
    }

    void Renderer::DeactivatePostProcessShader(const std::string &shaderName)
    {
        state.Post.Disable(shaderName);
    }

    void Renderer::InitializeGPUScreen()
    {
        if (!state.HasContext)
            return;
        GPUScreen &s = state.Screen;

        float screenQuadVertices[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f};

        s.Shader = ShaderSystem::LoadShader("Screen.glsl");

        s.Array = std::make_unique<VertexArray>();
        s.Array->GenerateVertexBuffer(screenQuadVertices, sizeof(screenQuadVertices));
        s.Array->GetVertexBuffer()->AddLayout(0, 0, 2);
        s.Array->GetVertexBuffer()->AddLayout(1, 2, 2);

        {
            FramebufferConfiguration config;
            config.Width = state.Viewport.Width;
            config.Height = state.Viewport.Height;
            config.Passes.push_back({.Type = RPTextureType::Rgb});
            config.Passes.push_back({.Type = RPTextureType::Depth});
            s.Buffer = std::make_unique<Framebuffer>(config);
        }

        {
            FramebufferConfiguration config;
            config.Width = state.Viewport.Width;
            config.Height = state.Viewport.Height;
            config.Passes.push_back({.Type = RPTextureType::Rgb});
            config.Passes.push_back({.Type = RPTextureType::Depth});
            s.PostBuffer = std::make_unique<Framebuffer>(config);
        }
    }

    void Renderer::BeginGPUScreenFrame()
    {
        if (!state.HasContext)
            return;
        GPUScreen &s = state.Screen;
        s.Buffer->Bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::EndGPUScreenFrame()
    {
        if (!state.HasContext)
            return;
        GPUScreen &s = state.Screen;
        s.Buffer->Unbind();
        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::ResizeGPUScreen()
    {
        if (!state.HasContext)
            return;
        GPUScreen &s = state.Screen;
        s.Buffer->Resize(state.Viewport.Width, state.Viewport.Height);
        s.PostBuffer->Resize(state.Viewport.Width, state.Viewport.Height);
    }
}
