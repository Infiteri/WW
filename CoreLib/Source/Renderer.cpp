#include "Renderer.h"
#include "Windows.h"
#include "Shader.h"
#include <glad/glad.h>

namespace WW
{

    GLuint VAO, VBO;
    Shader *shader;
    Matrix4 projMatrix = Matrix4::Perspective(90.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    Matrix4 viewMatrix = Matrix4::Translate(Vector3(0.0f, 0.0f, -3.0f));
    float rotationAngle = 0.0f; // rotation accumulator

    void InitSquare()
    {
        shader = new Shader("Shader.glsl");

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

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void RenderSquare()
    {
        glEnable(GL_DEPTH_TEST); // ensure depth test is enabled

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();
        shader->Mat4(viewMatrix, "view");
        shader->Mat4(projMatrix, "projection");

        // Update rotation
        rotationAngle += 0.01f;
        Matrix4 model = Matrix4::RotateZYX(Vector3(rotationAngle, rotationAngle, rotationAngle));
        shader->Mat4(model, "model");

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void Renderer::Init()
    {
        gladLoadGL();
        glEnable(GL_DEPTH_TEST);
        InitSquare();
    }

    void Renderer::Render()
    {
        RenderSquare();
    }

    void Renderer::Shutdown()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        delete shader;
    }

    void Renderer::Viewport(float w, float h)
    {
        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

        Matrix4 projMatrix = Matrix4::Perspective(90.0f, w / h, 0.1f, 100.0f);
    }
}
