#include "Renderer.h"
#include "Windows.h"
#include "Shader.h"
#include "Math/Transform.h"
#include "Camera/PerspectiveCamera.h"
#include "Texture/Texture2D.h"
#include "Texture/TextureSystem.h"
#include "BatchRenderer.h"
#include <glad/glad.h>
#include <random>

namespace WW
{

    static Renderer::State state;

    VertexArray *bg;
    BatchRenderer batch;
    PerspectiveCamera camera;
    Shader *shader, *bgShader;
    Texture2D *tex;
    std::vector<Transform> cubeTransforms;
    std::vector<Material> cubeMaterials;

    void Renderer::Init()
    {
        gladLoadGL();
        TextureSystem::Init();
        shader = new Shader("Shader.glsl");
        bgShader = new Shader("BgShader.glsl");
        tex = TextureSystem::Get2D("image.jfif");

        InitializeGPUScreen();

        glEnable(GL_DEPTH_TEST);

        camera.SetFOV(90.0f);
        camera.SetPosition(Vector3(0.0f, 0.0f, -5.0f));
        camera.UpdateView();

        batch.Init();

        std::mt19937 rng(42);
        std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
        std::uniform_real_distribution<float> scaleDist(0.1f, 1.0f);
        std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);

        int numCubes = 2300;
        float spread = 20.0f;

        // Cube 1
        cubeTransforms.push_back(Transform{.Position = Vector3(-2.0f, 0.0f, 0.0f)});

        // Cube 2
        cubeTransforms.push_back(Transform{.Position = Vector3(2.0f, 0.0f, 0.0f)});
        // Assuming Material has public members
        Material mat0;
        mat0.r = 1;
        mat0.g = 1;
        mat0.b = 1;
        mat0.a = 1;
        mat0.ColorTexturePath = "image.jfif";
        cubeMaterials.push_back(mat0);

        Material mat1;
        mat1.r = 1;
        mat1.g = 1;
        mat1.b = 1;
        mat1.a = 1;
        mat1.ColorTexturePath = "port.jfif";

        cubeMaterials.push_back(mat1);

        float quadVertices[] = {
            // pos.xy   uv.xy
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f};

        bg = new VertexArray();
        bg->GenerateVertexBuffer(quadVertices, sizeof(quadVertices));
        bg->GetVertexBuffer()->AddLayout(0, 0, 2);
        bg->GetVertexBuffer()->AddLayout(1, 2, 2);
    }

    void Renderer::Render()
    {
        BeginGPUScreenFrame();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        bgShader->Use();
        tex->Use(1);
        bgShader->Int(1, "uTexture");
        bg->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);

        camera.UpdateView();

        batch.Begin();
        shader->Use();
        int i = 0;
        for (auto &t : cubeTransforms)
            batch.RenderCube(t, cubeMaterials[i++]);

        batch.End(&camera, shader);

        EndGPUScreenFrame();

        // 3️⃣ Render framebuffer to screen
        FBRenderPass *pass = state.Screen.Buffer->GetRenderPass(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pass->Id);
        state.Screen.ScreenShader->Int(0, "uScreenTexture");
        state.Screen.ScreenShader->Use();
        state.Screen.Array->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Renderer::Viewport(float w, float h)
    {
        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

        camera.SetAspect(w / h);

        state.Viewport.Width = w;
        state.Viewport.Height = h;

        ResizeGPUScreen();
    }

    void Renderer::Shutdown() {}

    void Renderer::InitializeGPUScreen()
    {
        GPUScreen &s = state.Screen;
        {
            float screenQuadVertices[] = {
                -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

                -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};

            s.Array = std::make_unique<VertexArray>();
            s.Array->GenerateVertexBuffer(screenQuadVertices, sizeof(screenQuadVertices));
            s.Array->GetVertexBuffer()->AddLayout(0, 0, 2); // pos.xy
            s.Array->GetVertexBuffer()->AddLayout(1, 2, 2); // uv.xy
        }

        {
            FramebufferConfiguration config;
            config.Width = state.Viewport.Width;
            config.Height = state.Viewport.Height;
            config.Passes.push_back({.Type = RPTextureType::Rgb});   // color
            config.Passes.push_back({.Type = RPTextureType::Depth}); // depth
            s.Buffer = std::make_unique<Framebuffer>(config);
        }

        {
            s.ScreenShader = std::make_shared<Shader>("Screen.glsl");
        }
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
