#include "Display.h"
#include "Renderer.h"
#include <windows.h>
#include <stdio.h>

namespace WW
{
    static bool *s_running = nullptr; // Helper for WndProc

    WindowsDisplay::WindowsDisplay()
        : hwnd(nullptr), hdc(nullptr), hglrc(nullptr), running(true), parent(nullptr)
    {
        s_running = &running;
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
    }

    WindowsDisplay::~WindowsDisplay() {}

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_CLOSE && s_running)
            *s_running = false;

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    HWND WindowsDisplay::GetWallpaperWindow()
    {
        HWND progman = FindWindowA("Progman", nullptr);
        if (!progman)
        {
            MessageBoxA(nullptr, "Could not find Progman!", "Wallpaper Debug", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        SendMessageTimeoutA(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);

        HWND foundWorker = nullptr;
        EnumWindows([](HWND top, LPARAM lParam) -> BOOL
                    {
                        HWND defView = FindWindowExA(top, nullptr, "SHELLDLL_DefView", nullptr);
                        if (defView)
                        {
                            HWND *out = (HWND *)lParam;
                            *out = FindWindowExA(nullptr, top, "WorkerW", nullptr);
                            return FALSE;
                        }
                        return TRUE; }, (LPARAM)&foundWorker);

        if (!foundWorker)
            foundWorker = progman;

        return foundWorker;
    }

    void WindowsDisplay::Init()
    {
        // Get current screen size
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);

        wc = {};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"WallpaperGL";
        RegisterClass(&wc);

        parent = GetWallpaperWindow();

        hwnd = CreateWindowEx(
            WS_EX_NOACTIVATE | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
            wc.lpszClassName,
            L"WallpaperGL",
            WS_CHILD | WS_VISIBLE,
            0, 0,
            width,
            height,
            parent,
            nullptr,
            wc.hInstance,
            nullptr);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this); // store pointer for WM_SIZE

        SetParent(hwnd, parent);

        LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
        style &= ~WS_EX_APPWINDOW;
        style |= WS_EX_NOACTIVATE | WS_EX_TRANSPARENT;
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        hdc = GetDC(hwnd);
        running = true;

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
    }

    void WindowsDisplay::PreRender()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        Renderer::Viewport(width, height);
    }

    void WindowsDisplay::PostRender()
    {
        if (hdc)
            SwapBuffers(hdc);

        Sleep(16);
    }

    void WindowsDisplay::Shutdown()
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
