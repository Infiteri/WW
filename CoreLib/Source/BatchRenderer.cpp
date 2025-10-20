#include "BatchRenderer.h"

namespace WW
{
    std::unordered_map<std::string, int> textureSlotMap; // Texture path → GPU slot
    std::vector<Texture2D *> boundTextures;              // All bound textures for this frame
    static const int MaxTextureSlots = 16;               // Or glGetInt(GL_MAX_TEXTURE_IMAGE_UNITS)

    void BatchRenderer::Init()
    {
        instanceCount = 0;

        // Cube vertex data: position (xyz) + UV (uv)
        float cubeVertices[] = {
            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            // left
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            // right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            // top
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f};

        // Cube VAO setup
        cubeVAO = new VertexArray();
        cubeVAO->GenerateVertexBuffer(cubeVertices, sizeof(cubeVertices));
        cubeVAO->GetVertexBuffer()->AddLayout(0, 0, 3); // position
        cubeVAO->GetVertexBuffer()->AddLayout(6, 3, 2); // UV

        // Instance buffer
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * MaxInstances, nullptr, GL_DYNAMIC_DRAW);

        cubeVAO->Bind();

        // Per-instance: model matrix (4 vec4s)
        for (int i = 0; i < 4; ++i)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE,
                                  sizeof(InstanceData),
                                  (void *)(offsetof(InstanceData, model) + sizeof(float) * 4 * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        // Per-instance: material color
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
                              sizeof(InstanceData),
                              (void *)offsetof(InstanceData, material));
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)(offsetof(InstanceData, material) + sizeof(float) * 4));
        glVertexAttribDivisor(7, 1);
    }

    void BatchRenderer::Begin()
    {
        instanceCount = 0;
        textureSlotMap.clear();
        boundTextures.clear();

        TextureSystem::ActivateDefault();
        textureSlotMap["__default__"] = 0;
        boundTextures.push_back(TextureSystem::GetDefaultTexture2D());
    }

    void BatchRenderer::RenderCube(const Transform &t, const Material &mat)
    {
        if (instanceCount >= MaxInstances)
            return;

        int slot = 0; // Default texture slot

        if (!mat.ColorTexturePath.empty())
        {
            auto it = textureSlotMap.find(mat.ColorTexturePath);
            if (it != textureSlotMap.end())
            {
                slot = it->second;
            }
            else
            {
                slot = (int)boundTextures.size();
                if (slot >= MaxTextureSlots)
                {
                    slot = 0;
                }
                else
                {
                    Texture2D *tex = TextureSystem::Get2D(mat.ColorTexturePath);
                    tex->Use(slot);
                    textureSlotMap[mat.ColorTexturePath] = slot;
                    boundTextures.push_back(tex);
                }
            }
        }

        instanceData[instanceCount].model = t.GetMatrix();
        instanceData[instanceCount].material = {
            .r = mat.r,
            .g = mat.g,
            .b = mat.b,
            .a = mat.a,
            .ColorTextureIndex = slot};

        instanceCount++;
    }

    void BatchRenderer::End(PerspectiveCamera *cam, Shader *shader)
    {
        if (instanceCount == 0)
            return;

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        instanceCount * sizeof(InstanceData),
                        instanceData);

        shader->Use();

        // Bind all textures to their respective slots
        for (int i = 0; i < (int)boundTextures.size(); i++)
        {
            boundTextures[i]->Use(i);
            shader->Int(i, ("uTextures[" + std::to_string(i) + "]").c_str());
        }

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
