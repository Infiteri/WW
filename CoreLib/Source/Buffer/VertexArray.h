#pragma once

#include "Buffer/Buffer.h"
#include <memory>
#include <stdint.h>

namespace WW
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind();
        void Unbind();

        inline std::shared_ptr<Buffer> GetVertexBuffer() { return vertexBuffer; };
        inline std::shared_ptr<Buffer> GetIndexBuffer() { return indexBuffer; };

        void GenerateVertexBuffer(void *data, uint64_t size);
        void GenerateIndexBuffer(void *data, uint64_t size);

        inline uint32_t GetID() const { return id; };

    private:
        uint32_t id;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;
    };
} // namespace Core
