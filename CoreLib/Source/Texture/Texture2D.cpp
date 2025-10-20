#include "Texture2D.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace WW

{

    void Texture2D::CleanID()
    {
        if (id != 0)
            glDeleteTextures(1, &id);

        id = 0;
    }

    void Texture2D::Load(int width, int height, int channels, uint8_t *data)
    {
        GLenum format = channels == 3 ? GL_RGB : GL_RGBA;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        GLenum wrap = GL_REPEAT;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        // Use linear filtering for smooth texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    Texture2D::Texture2D()
    {
    }

    Texture2D::~Texture2D()
    {
        CleanID();
    }

    void Texture2D::Load()
    {
        CleanID();

        glGenTextures(1, &id);
        Bind();

        uint8_t data[] = {255, 255, 255, 255};
        Load(1, 1, 4, data);
    }

    void Texture2D::Load(const std::string &path)
    {
        CleanID();
        glGenTextures(1, &id);
        Bind();
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        uint8_t *data = stbi_load(path.c_str(), &width, &height, &channels, 4); // force RGBA

        if (!data)
        {
            // fallback: 1x1 pink pixel
            uint8_t fallback[] = {255, 255, 255, 255};
            Load(1, 1, 4, fallback);
            return;
        }

        Load(width, height, 4, data);
        stbi_image_free(data);
    }

    void Texture2D::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture2D::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Use(int layer)
    {
        glActiveTexture(GL_TEXTURE0 + layer);
        Bind();
    }
}