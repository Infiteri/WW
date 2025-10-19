#include "Framebuffer.h"

#include <cstddef>
#include <glad/glad.h>

namespace WW
{
    Framebuffer::Framebuffer(const FramebufferConfiguration &cfg)
    {
        state = cfg;
        Create();
    }

    Framebuffer::~Framebuffer()
    {
        DestroyForResize();

        state.Passes.clear();
    }

    void Framebuffer::DestroyForResize()
    {
        glDeleteFramebuffers(1, &id);
        for (auto &pass : state.Passes)
        {
            if (pass.Type == RPTextureType::Depth)
                glDeleteRenderbuffers(1, &pass.Id);
            else
                glDeleteTextures(1, &pass.Id);

            pass.Id = 0;
            pass.Index = 0;
        }
    }

    void Framebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, id); }

    void Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void Framebuffer::Create()
    {
        glGenFramebuffers(1, &id);
        Bind();

        for (size_t i = 0; i < state.Passes.size(); i++)
        {
            FBRenderPass &pass = state.Passes[i];
            pass.Index = i;

            if (pass.Type == RPTextureType::Depth)
            {
                glGenRenderbuffers(1, &pass.Id);
                glBindRenderbuffer(GL_RENDERBUFFER, pass.Id);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, state.Width,
                                      state.Height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                          GL_RENDERBUFFER, pass.Id);
            }
            else
            {
                glGenTextures(1, &pass.Id);
                glBindTexture(GL_TEXTURE_2D, pass.Id);

                GLenum internalFormat, format;

                switch (pass.Type)
                {
                case RPTextureType::Rgba8:
                    internalFormat = GL_RGBA8;
                    format = GL_RGBA;
                    break;
                case RPTextureType::Rgb:
                    internalFormat = GL_RGB8;
                    format = GL_RGB;
                    break;
                default:
                    break;
                }

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, state.Width, state.Height, 0, format,
                             GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glBindTexture(GL_TEXTURE_2D, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                       pass.Id, 0);
            }
        }

        Unbind();
    }

    FBRenderPass *Framebuffer::GetRenderPass(int index)
    {
        if (index >= 0 && index < (int)state.Passes.size())
            return &state.Passes[index];

        return nullptr;
    };

    void Framebuffer::Resize(float w, float h)
    {
        if (id)
            DestroyForResize();

        state.Width = w;
        state.Height = h;

        Create();
    }
} // namespace Core
