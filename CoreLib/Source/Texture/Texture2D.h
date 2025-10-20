#pragma once

#include "Texture.h"
#include <string>

namespace WW
{
    class Texture2D : public Texture
    {
    public:
        Texture2D();
        ~Texture2D();

        void Load();
        void Load(const std::string& path);

        void Bind();
        void Unbind();
        void Use(int layer);

    private:
        void CleanID();
        void Load(int width, int height, int channels, uint8_t *data);
    };
}