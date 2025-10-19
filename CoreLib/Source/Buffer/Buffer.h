#pragma once

#include <vector>
#include <stdint.h>

namespace WW
{
    enum class BufferType : uint8_t
    {
        Vertex = 0,
        Index = 1
    };

    enum class BufferDataType : uint8_t
    {
        Static,
        Dynamic
    };

    class Buffer
    {
    public:
        Buffer(BufferType bufferType);
        ~Buffer();

        void Bind();
        void Unbind();

        void Draw();
        void BufferData(void *data, uint64_t size, BufferDataType dataType = BufferDataType::Static);

        void AddLayout(int location, int offset, int size);
        void BindLayout();

        inline uint32_t GetID() { return id; };
        inline BufferType GetType() { return type; };

    private:
        struct Layout
        {
            int Loc, Off, Size;
        };

    private:
        BufferType type;
        uint32_t id, stride, size;
        std::vector<Layout> layout;
    };

} // namespace Core
