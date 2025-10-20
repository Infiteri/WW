#pragma once

#include "Color.h"
#include "Shader.h"
#include "Texture/Texture2D.h"
#include "Buffer/VertexArray.h"
#include <memory>
#include <variant>

namespace WW
{

    enum class BackgroundType
    {
        SolidColor,
        Texture,
        Shader
    };

    class BackgroundLayer
    {
    public:
        BackgroundLayer();
        ~BackgroundLayer();

        void Render();

        void SetTypeSolidColor(const Color &c);
        void SetTypeTexture(const std::string &texturePath);
        void SetTypeShader(const std::string &shaderPath);

        void SetUniform(const std::string &name, int v);
        void SetUniform(const std::string &name, float v);
        void SetUniform(const std::string &name, const Color &v);
        void SetUniform(const std::string &name, const Vector2 &v);
        void SetUniform(const std::string &name, const Vector3 &v);
        void SetUniform(const std::string &name, const Matrix4 &v);

        inline BackgroundType GetType() const { return type; }

        inline Color &GetSolidColor() { return color; }

    private:
        BackgroundType type;

    private:
        // General properties
        // note: This array is for the screen as the image and shader both render here and the color will as well
        std::unique_ptr<VertexArray> array;
        std::shared_ptr<Shader> screenShader;

        // Solid color background properties
        Color color;

        // Texture background properties
        std::shared_ptr<Texture2D> texture;

        // Shader background properties
        std::shared_ptr<Shader> customShader;

    private:
        void EnsureBackgroundState();

        void UploaUniforms(std::shared_ptr<Shader> shader);

    private:
        struct UniformValue
        {
            using ValueType = std::variant<int, float, Color, Vector2, Vector3, Matrix4>;
            ValueType value;
        };
        std::unordered_map<std::string, UniformValue> uniforms;
    };

}