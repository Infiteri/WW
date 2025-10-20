#pragma once

#include <string>
#include <unordered_map>
#include "Texture.h"
#include "Texture2D.h"

namespace WW
{
    class TextureSystem
    {
    public:
        static void Init();
        static void Shutdown();

        static Texture2D *GetDefaultTexture2D();

        static Texture2D *Get2D(const std::string &path);
        static Texture2D *Load2D(const std::string &path);

        static void ActivateDefault();

    public:
        struct State
        {
            std::unordered_map<std::string, Texture *> Textures;
            Texture2D DefaultTexture2D;
        };
    };
}
