#pragma once

#include <stdint.h>

namespace WW
{
    class Texture
    {
    public:
        Texture() {}
        virtual ~Texture() {}

        virtual void Load() {}

        virtual void Bind() {}
        virtual void Unbind() {}
        virtual void Use(int layer) {}

        inline uint32_t GetID() const { return id; }

    protected:
        uint32_t id;
    };
}