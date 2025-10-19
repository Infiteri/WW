#pragma once

#include <Windows.h>
#include "Core/IDisplay.h"

namespace WW
{
    class WindowsDisplay : IDisplay
    {
    private:
        HWND hwnd;
        HDC hdc;
        HGLRC hglrc;
        bool running;

        WNDCLASS wc;
        HWND parent;

        HWND GetWallpaperWindow();

    public:
        WindowsDisplay();
        ~WindowsDisplay() override;

        void Init() override;
        void PreRender() override;
        void PostRender() override;
        void Shutdown() override;

        bool WantsUpdate() override { return running; }
    };
}