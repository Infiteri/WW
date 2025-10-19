#pragma once

namespace WW
{
    class Renderer
    {
    public:
        static void Viewport(float w, float h);

        static void Init();
        static void Render();
        static void Shutdown();
    };
}