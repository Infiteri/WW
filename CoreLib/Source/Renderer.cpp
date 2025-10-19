#include "Renderer.h"
#include "Windows.h"
#include <glad/glad.h>

namespace WW
{
    // --- OpenGL ---
    GLuint shaderProgram;
    GLuint VAO, VBO;

    // Vertex shader (2D, no transforms)
    const char *vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

    // Fragment shader (white)
    const char *fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // white
}
)";

    // Compile shader utility
    GLuint CompileShader(GLenum type, const char *src)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char info[512];
            glGetShaderInfoLog(shader, 512, nullptr, info);
            MessageBoxA(nullptr, info, "Shader Compile Error", MB_OK | MB_ICONERROR);
        }
        return shader;
    }

    // Initialize square (2 triangles, no transforms)
    void InitSquare()
    {
        float vertices[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,
            -0.5f, -0.5f};

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            char info[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
            MessageBoxA(nullptr, info, "Shader Link Error", MB_OK | MB_ICONERROR);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Render the square
    void RenderSquare()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Renderer::Init()
    {
        gladLoadGL();

        InitSquare();
    }

    void Renderer::Render()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background
        glClear(GL_COLOR_BUFFER_BIT);
        RenderSquare();
    }

    void Renderer::Shutdown()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }

}