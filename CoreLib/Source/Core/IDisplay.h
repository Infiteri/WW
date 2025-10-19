#pragma once

namespace WW
{
    class IDisplay
    {
    protected:
        int width = 0;
        int height = 0;

    public:
        IDisplay() = default;
        virtual ~IDisplay() = default;

        virtual void Init() = 0;
        virtual void PreRender() = 0;
        virtual void PostRender() = 0;
        virtual void Shutdown() = 0;
        virtual bool WantsUpdate() = 0;

        void SetSize(int w, int h)
        {
            width = w;
            height = h;
        }
        inline int GetWidth() const { return width; }
        inline int GetHeight() const { return height; }
    };
}
