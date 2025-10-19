#include "Display.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <windows.h>

namespace WW
{
    int WindowsMain()
    {
        WindowsDisplay display;
        display.Init();

        Renderer::Init();

        while (display.WantsUpdate())
        {
            display.PreRender();
            Renderer::Render();
            display.PostRender();
        }

        Renderer::Shutdown();
        display.Shutdown();

        return 0;
    }
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    return WW::WindowsMain();
}
