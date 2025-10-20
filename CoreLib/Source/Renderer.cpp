#include "Renderer.h"
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

    static std::unique_ptr<BackgroundLayer> backgroundLayer, bg2;
    static std::vector<Transform> cubeTransforms;
    static std::vector<Material> cubeMaterials;
    static std::shared_ptr<Texture2D> tex;

    Color lastColor; // last frame color used
    float lerpFactor = 0.05f;

    void Renderer::Init()
    {
        gladLoadGL();
        TextureSystem::Init();

        bg2 = std::make_unique<BackgroundLayer>();
        bg2->SetTypeTexture("image.jfif");

        backgroundLayer = std::make_unique<BackgroundLayer>();
        backgroundLayer->SetTypeShader("CS.glsl");

        state.MainShader = ShaderSystem::LoadShader("Shader.glsl");

        InitializeGPUScreen();

        glEnable(GL_DEPTH_TEST);

        state.Camera.SetFOV(90.0f);
        state.Camera.SetPosition(Vector3(0.0f, 0.0f, -5.0f));
        state.Camera.UpdateView();

        state.Batch.Init();

        cubeTransforms.push_back(Transform{.Position = Vector3(-2.0f, 0.0f, 0.0f)});
        cubeTransforms.push_back(Transform{.Position = Vector3(2.0f, 0.0f, 0.0f)});

        Material mat0;
        mat0.Color = {255, 255, 255, 255};
        mat0.ColorTexturePath = "image.jfif";
        cubeMaterials.push_back(mat0);

        Material mat1;
        mat1.Color = {255, 255, 255, 255};
        mat1.ColorTexturePath = "port.jfif";
        cubeMaterials.push_back(mat1);
    }

    void Renderer::Render()
    {
        BeginGPUScreenFrame();

        state.Camera.UpdateView();

        // Render the background
        glDisable(GL_DEPTH_TEST);
        backgroundLayer->Render();
        glEnable(GL_DEPTH_TEST);

        // Render the main objects in 3D
        state.Batch.Begin();
        state.MainShader->Use();
        int i = 0;
        for (auto &t : cubeTransforms)
            state.Batch.RenderCube(t, cubeMaterials[i++]);
        state.Batch.End(&state.Camera, state.MainShader.get());

        // End frame
        EndGPUScreenFrame();

        FBRenderPass *pass = state.Screen.Buffer->GetRenderPass(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pass->Id);
        state.Screen.Shader->Use();
        state.Screen.Shader->Int(0, "uScreenTexture");
        state.Screen.Array->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Renderer::Viewport(float w, float h)
    {
        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
        state.Camera.SetAspect(w / h);
        state.Viewport.Width = w;
        state.Viewport.Height = h;
        ResizeGPUScreen();
    }

    void Renderer::Shutdown() {}

    void Renderer::InitializeGPUScreen()
    {
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

        FramebufferConfiguration config;
        config.Width = state.Viewport.Width;
        config.Height = state.Viewport.Height;
        config.Passes.push_back({.Type = RPTextureType::Rgb});
        config.Passes.push_back({.Type = RPTextureType::Depth});
        s.Buffer = std::make_unique<Framebuffer>(config);
    }

    void Renderer::BeginGPUScreenFrame()
    {
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
        GPUScreen &s = state.Screen;
        s.Buffer->Unbind();
        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::ResizeGPUScreen()
    {
        GPUScreen &s = state.Screen;
        s.Buffer->Resize(state.Viewport.Width, state.Viewport.Height);
    }
}
