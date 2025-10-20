#pragma once

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Buffer/VertexArray.h"
#include "Shader.h"
#include "Camera/PerspectiveCamera.h"
#include "Texture/TextureSystem.h"
#include "Color.h"
#include <glad/glad.h>
#include <string>

namespace WW
{
    struct Material
    {
        Color Color;
        std::string ColorTexturePath;

        Material() : Color(255, 255, 255, 255), ColorTexturePath("") {}
    };

    struct GPUMaterial
    {
        float r, g, b, a;
        float ColorTextureIndex;
    };

    class BatchRenderer
    {
    public:
        void Init();
        void Begin();
        void RenderCube(const Transform &t, const Material &mat);
        void End(PerspectiveCamera *cam, Shader *shader);
        void Shutdown();

    private:
        static const int MaxInstances = 10000;
        static const int MaxTextureSlots = 8;

        struct InstanceData
        {
            Matrix4 model;
            GPUMaterial material;
        };

        InstanceData instanceData[MaxInstances];
        int instanceCount;

        VertexArray *cubeVAO;
        uint32_t instanceVBO;
    };
}
