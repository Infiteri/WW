#include "TextureSystem.h"

namespace WW
{
    static TextureSystem::State state;

    void TextureSystem::Init()
    {
        state.DefaultTexture2D.Load();
    }

    void TextureSystem::Shutdown()
    {
        for (auto &pair : state.Textures)
            delete pair.second;

        state.Textures.clear();
    }

    Texture2D *TextureSystem::GetDefaultTexture2D()
    {
        return &state.DefaultTexture2D;
    }

    Texture2D *TextureSystem::Get2D(const std::string &path)
    {
        auto it = state.Textures.find(path);
        if (it != state.Textures.end())
            return (Texture2D *)it->second;

        return Load2D(path);
    }

    Texture2D *TextureSystem::Load2D(const std::string &path)
    {
        auto it = state.Textures.find(path);
        if (it != state.Textures.end())
            return (Texture2D *)it->second;

        Texture2D *tex = new Texture2D();
        tex->Load(path);
        state.Textures[path] = tex;
        return tex;
    }

    void TextureSystem::ActivateDefault()
    {
        state.DefaultTexture2D.Use(0);
    }
}