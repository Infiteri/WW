#include "TextureSystem.h"

namespace WW
{
    static TextureSystem::State state;

    void TextureSystem::Init()
    {
        state.DefaultTexture2D = std::make_shared<Texture2D>();
        state.DefaultTexture2D->Load();
    }

    void TextureSystem::Shutdown()
    {
        state.Textures.clear();
        state.DefaultTexture2D.reset();
    }

    std::shared_ptr<Texture2D> TextureSystem::GetDefaultTexture2D()
    {
        return state.DefaultTexture2D;
    }

    std::shared_ptr<Texture2D> TextureSystem::Get2D(const std::string &path)
    {
        auto it = state.Textures.find(path);
        if (it != state.Textures.end())
            return std::static_pointer_cast<Texture2D>(it->second);

        return Load2D(path);
    }

    std::shared_ptr<Texture2D> TextureSystem::Load2D(const std::string &path)
    {
        auto it = state.Textures.find(path);
        if (it != state.Textures.end())
            return std::static_pointer_cast<Texture2D>(it->second);

        auto tex = std::make_shared<Texture2D>();
        tex->Load(path);
        state.Textures[path] = tex;
        return tex;
    }

    void TextureSystem::ActivateDefault()
    {
        if (state.DefaultTexture2D)
            state.DefaultTexture2D->Use(0);
    }
}
