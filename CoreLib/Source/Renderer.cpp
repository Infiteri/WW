#include "Renderer.h"
#include "Windows.h"
#include "Shader.h"
#include "Math/Transform.h"
#include "Camera/PerspectiveCamera.h"
#include <glad/glad.h>

namespace WW
{

    VertexArray *array;
    Shader *shader;
    PerspectiveCamera camera;
    Transform transform;

    static Renderer::State state;

    void InitSquare()
    {
        shader = new Shader("Shader.glsl");
        camera.SetPosition(Vector3(0.0f, 0.0f, -3.0f));
        camera.UpdateView();

        float vertices[] = {
            // front
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,

            // back
            -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            // left
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            // right
            0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,

            // top
            -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,

            // bottom
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f};

        array = new VertexArray();
        array->GenerateVertexBuffer(vertices, sizeof(vertices));
        array->GetVertexBuffer()->AddLayout(0, 0, 3); // pos.xyz
    }

    void RenderSquare()
    {
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();
        shader->Mat4(camera.GetView(), "view");
        shader->Mat4(camera.GetProjection(), "projection");

        // Update rotation
        transform.Rotation.y += 1.0f;
        transform.Rotation.x += 1.0f;
        transform.Rotation.z += 1.0f;
        shader->Mat4(transform.GetMatrix(), "model");

        array->Bind();
        array->GetVertexBuffer()->Bind();
        array->GetVertexBuffer()->Draw();
    }

    void Renderer::Init()
    {
        gladLoadGL();
        InitializeGPUScreen();

        glEnable(GL_DEPTH_TEST);
        InitSquare();
    }

    void Renderer::Render()
    {
        BeginGPUScreenFrame();

        RenderSquare();

        EndGPUScreenFrame();

        FBRenderPass *pass = state.Screen.Buffer->GetRenderPass(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pass->Id);
        state.Screen.ScreenShader->Int(0, "uScreenTexture");
        state.Screen.ScreenShader->Use();
        state.Screen.Array->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Renderer::Shutdown()
    {
        delete array;
        delete shader;
    }

    void Renderer::Viewport(float w, float h)
    {
        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

        camera.SetAspect(w / h);

        state.Viewport.Width = w;
        state.Viewport.Height = h;

        ResizeGPUScreen();
    }

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
