#include "BatchRenderer.h"

namespace WW
{
    void BatchRenderer::Init()
    {
        instanceCount = 0;

        // Cube vertices (36 vertices)
        float cubeVertices[] = {
            -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

        cubeVAO = new VertexArray();
        cubeVAO->GenerateVertexBuffer(cubeVertices, sizeof(cubeVertices));
        cubeVAO->GetVertexBuffer()->AddLayout(0, 0, 3); // pos.xyz

        // Instance buffer
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * MaxInstances, nullptr, GL_DYNAMIC_DRAW);

        cubeVAO->Bind();

        // Matrix columns: locations 1-4
        for (int i = 0; i < 4; ++i)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)(offsetof(InstanceData, model) + sizeof(float) * 4 * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        // Material: location 5
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)offsetof(InstanceData, material));
        glVertexAttribDivisor(5, 1);
    }

    void BatchRenderer::Begin()
    {
        instanceCount = 0;
    }

    void BatchRenderer::RenderCube(const Transform &t, const Material &mat)
    {
        if (instanceCount >= MaxInstances)
            return;
        instanceData[instanceCount].model = t.GetMatrix();
        instanceData[instanceCount].material = mat;
        instanceCount++;
    }

    void BatchRenderer::End(PerspectiveCamera *cam, Shader *shader)
    {
        if (instanceCount == 0)
            return;

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstanceData) * instanceCount, instanceData);

        shader->Use();
        shader->Mat4(cam->GetView(), "view");
        shader->Mat4(cam->GetProjection(), "projection");

        cubeVAO->Bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, instanceCount);
    }

    void BatchRenderer::Shutdown()
    {
        delete cubeVAO;
        glDeleteBuffers(1, &instanceVBO);
    }
}
