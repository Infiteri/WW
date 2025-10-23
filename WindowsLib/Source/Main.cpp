#include "Renderer.h"
#include "Display.h"
#include "Platform/Platform.h"
#include "Scene/Components.h"
#include "Scene/Object.h"
#include "Scene/Scene.h"
#include <glad/glad.h>
#include <windows.h>
#include <iostream>

namespace WW
{

    static std::shared_ptr<Scene> CreateTestScene()
    {
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        std::shared_ptr<Object> testObj = scene->CreateObject("Test Object");
        std::shared_ptr<Object> testObj2 = scene->CreateObject("Test2 Object");

        auto bg = testObj->AddComponent<BackgroundLayerComponent>();
        bg->GetLayer()->SetTypeSolidColor({0, 120, 0, 255});

        auto shape = testObj2->AddComponent<ShapeComponent>();
        shape->GetTransform().Position = {-2, 0, 0};

        auto shape2 = testObj2->AddComponent<ShapeComponent>();
        shape2->GetTransform().Position = {4, 0, 0};
        return scene;
    }

    int WindowsMain()
    {
        WindowsDisplay display;
        display.Init();

        Renderer::Init();
        Renderer::ActivateScene(CreateTestScene());

        while (display.WantsUpdate())
        {
            display.PreRender();
            Renderer::BeginFrame();
            Renderer::Render();
            Renderer::EndFrame();
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
