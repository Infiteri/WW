#include "BackgroundLayer.h"
#include "ShaderSystem.h"
#include "Debug/WWDebug.h"
#include "Texture/TextureSystem.h"
#include <glad/glad.h>

namespace WW
{

    void BackgroundLayer::EnsureBackgroundState()
    {
        if (!array)
        {
            static float quadVertices[] = {
                -1.0f, 1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f};

            array = std::make_unique<VertexArray>();
            array->GenerateVertexBuffer(quadVertices, sizeof(quadVertices));
            array->GetVertexBuffer()->AddLayout(0, 0, 2);
            array->GetVertexBuffer()->AddLayout(1, 2, 2);
        }

        if (!screenShader)
            screenShader = ShaderSystem::GetShader("BgShader.glsl");
    }

    BackgroundLayer::BackgroundLayer()
        : type(BackgroundType::SolidColor), array(nullptr)
    {
        EnsureBackgroundState();
    }

    BackgroundLayer::~BackgroundLayer()
    {
    }

    void BackgroundLayer::Render()
    {
        EnsureBackgroundState();
        if (!array || !screenShader)
            return;

        switch (type)
        {
        case BackgroundType::SolidColor:
        {
            Color c = color.Normalized();
            glClearColor(c.r, c.g, c.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        break;

        case BackgroundType::Texture:
        {
            if (!screenShader || !texture)
                return;

            screenShader->Use();
            texture->Use(0);
            screenShader->Int(0, "uTexture");

            array->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        break;

        case BackgroundType::Shader:
        {
            if (!customShader)
            {
                WW_LOG_LVL("ERROR", "BackgroundLayer::Render: Custom shader not set for Shader background type.");
                SetTypeSolidColor(Color(0, 0, 0, 255));
                return;
            }

            customShader->Use();
            array->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        break;

        default:
            break;
        }
    }

    void BackgroundLayer::SetTypeSolidColor(const Color &c)
    {
        type = BackgroundType::SolidColor;
        color = c;
    }

    void BackgroundLayer::SetTypeTexture(const std::string &texturePath)
    {
        type = BackgroundType::Texture;
        texture = TextureSystem::Get2D(texturePath);
    }

    void BackgroundLayer::SetTypeShader(const std::string &shaderPath)
    {
        type = BackgroundType::Shader;
        customShader = ShaderSystem::GetShader(shaderPath);
    }
}