#include "BackgroundLayer.h"
#include "ShaderSystem.h"
#include "Debug/WWDebug.h"
#include "Texture/TextureSystem.h"
#include <glad/glad.h>

namespace WW
{

    static float frameCount = 0;
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

    void BackgroundLayer::SetUniform(const std::string &name, int v) { uniforms[name].value = v; }
    void BackgroundLayer::SetUniform(const std::string &name, float v) { uniforms[name].value = v; }
    void BackgroundLayer::SetUniform(const std::string &name, const Color &v) { uniforms[name].value = v; }
    void BackgroundLayer::SetUniform(const std::string &name, const Vector2 &v) { uniforms[name].value = v; }
    void BackgroundLayer::SetUniform(const std::string &name, const Vector3 &v) { uniforms[name].value = v; }
    void BackgroundLayer::SetUniform(const std::string &name, const Matrix4 &v) { uniforms[name].value = v; }

    void BackgroundLayer::UploaUniforms(std::shared_ptr<Shader> shader)
    {
        if (!shader)
            return;

        for (auto &[name, uniform] : uniforms)
        {
            std::visit([&](auto &&val)
                       {
        using T = std::decay_t<decltype(val)>;

        if constexpr (std::is_same_v<T, int>)
        {
            shader->Int(val, name.c_str());
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            shader->Float(val, name.c_str());
        }
        else if constexpr (std::is_same_v<T, Color>)
        {
            shader->Color4(val, name.c_str());
        }
        else if constexpr (std::is_same_v<T, Vector2>)
        {
            shader->Vec2(val, name.c_str());
        }
        else if constexpr (std::is_same_v<T, Vector3>)
        {
            shader->Vec3(val, name.c_str());
        }
        else if constexpr (std::is_same_v<T, Matrix4>)
        {
            shader->Mat4(val, name.c_str());
        } }, uniform.value);
        }
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

        frameCount += 1.0/60;

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
            UploaUniforms(customShader);
            customShader->Float(frameCount, "uTime");
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