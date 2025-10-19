#include "Renderer.h"
#include "Windows.h"
#include "Shader.h"
#include "Math/Transform.h"
#include "Camera/PerspectiveCamera.h"
#include "BatchRenderer.h"
#include <glad/glad.h>
#include <random>

namespace WW
{

    static Renderer::State state;

    BatchRenderer batch;
    PerspectiveCamera camera;
    Shader *shader;
    std::vector<Transform> cubeTransforms;
    std::vector<Material> cubeMaterials;

    void Renderer::Init()
    {
        gladLoadGL();
        shader = new Shader("Shader.glsl");

        InitializeGPUScreen();

        glEnable(GL_DEPTH_TEST);

        camera.SetFOV(90.0f);
        camera.SetPosition(Vector3(0.0f, 0.0f, -20.0f));
        camera.UpdateView();

        batch.Init();

        std::mt19937 rng(42); // deterministic seed for reproducibility
        std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
        std::uniform_real_distribution<float> scaleDist(0.1f, 1.0f);
        std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);

        int numCubes = 200;   // number of cubes
        float spread = 20.0f; // how far cubes can be from the origin

        for (int i = 0; i < numCubes; i++)
        {
            Transform t;
            t.Position = Vector3(
                ((float)rand() / RAND_MAX - 0.5f) * spread,
                ((float)rand() / RAND_MAX - 0.5f) * spread,
                ((float)rand() / RAND_MAX - 0.5f) * spread);
            t.Rotation = Vector3(rand() % 360, rand() % 360, rand() % 360);
            float f = 0.3f + ((float)rand() / RAND_MAX) * 0.7f;
            t.Scale = Vector3(f, f, f);

            Material m;
            m.r = (float)rand() / RAND_MAX;
            m.g = (float)rand() / RAND_MAX;
            m.b = (float)rand() / RAND_MAX;
            m.a = 1.0f;

            cubeTransforms.push_back(t);
            cubeMaterials.push_back(m);
        }
    }

    void Renderer::Render()
    {
        BeginGPUScreenFrame();

        batch.Begin();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.SetRotation({camera.GetRotation().x, camera.GetRotation().y, camera.GetRotation().z});
        camera.UpdateView();

        int i = 0;
        for (auto &t : cubeTransforms)
            batch.RenderCube(t, cubeMaterials[i++]);

        batch.End(&camera, shader); // Flush all cubes in one draw call

        EndGPUScreenFrame();

        // Render framebuffer to screen
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
