#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Texture.h"
#include "Texture2D.h"

namespace WW
{
    class TextureSystem
    {
    public:
        static void Init();
        static void Shutdown();

        static std::shared_ptr<Texture2D> GetDefaultTexture2D();

        static std::shared_ptr<Texture2D> Get2D(const std::string &path);
        static std::shared_ptr<Texture2D> Load2D(const std::string &path);

        static void ActivateDefault();

    public:
        struct State
        {
            std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
            std::shared_ptr<Texture2D> DefaultTexture2D;
        };
    };
}
