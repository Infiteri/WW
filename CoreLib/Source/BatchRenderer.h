#pragma once

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Buffer/VertexArray.h"
#include "Shader.h"
#include "Camera/PerspectiveCamera.h"
#include <glad/glad.h>

namespace WW
{
    struct Material
    {
        float r, g, b, a;
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

        struct InstanceData
        {
            Matrix4 model;
            Material material;
        };

        InstanceData instanceData[MaxInstances];
        int instanceCount;

        VertexArray *cubeVAO;
        uint32_t instanceVBO;
    };
}
