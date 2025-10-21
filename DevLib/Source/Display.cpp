#include "Display.h"
#include "Renderer.h"
#include <windows.h>
#include <stdio.h>

namespace WW
{
    static bool *s_running = nullptr; // Helper for WndProc

    DevDisplay::DevDisplay()
        : hwnd(nullptr), hdc(nullptr), hglrc(nullptr), running(true), parent(nullptr)
    {
        s_running = &running;
        width = 1280;
        height = 720;
    }

    DevDisplay::~DevDisplay() {}

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CLOSE:
            if (s_running)
                *s_running = false;
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            int w = LOWORD(lParam);
            int h = HIWORD(lParam);
            Renderer::Viewport(w, h);
            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void DevDisplay::Init()
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        wc = {};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"DevWindow";
        wc.style = CS_OWNDC; // ensure each window gets its own DC
        RegisterClass(&wc);

        hwnd = CreateWindowEx(
            0,
            wc.lpszClassName,
            L"WW Dev Window",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            nullptr, nullptr,
            wc.hInstance, nullptr);

        if (!hwnd)
        {
            MessageBoxA(nullptr, "Failed to create window!", "Error", MB_OK | MB_ICONERROR);
            return;
        }

        hdc = GetDC(hwnd);

        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int pf = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pf, &pfd);

        hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    }

    void DevDisplay::PreRender()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void DevDisplay::PostRender()
    {
        if (hdc)
            SwapBuffers(hdc);
    }

    void DevDisplay::Shutdown()
    {
        if (hwnd && hdc)
            ReleaseDC(hwnd, hdc);
        if (hglrc)
        {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(hglrc);
            hglrc = nullptr;
        }
        if (hwnd)
            DestroyWindow(hwnd);

        hwnd = nullptr;
        hdc = nullptr;
        running = false;
    }
}
