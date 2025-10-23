#include "Renderer.h"
#include "Platform/Platform.h"
#include "Scene/Components.h"
#include "Scene/Object.h"
#include "Scene/Scene.h"
#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <windows.h>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace WW
{
    static void OnResize(GLFWwindow *window, int width, int height)
    {
        Renderer::Viewport(static_cast<float>(width), static_cast<float>(height));
    }

    static void RenderGui()
    {
        ImGui::Begin("Post");
        if (ImGui::Button("Add"))
        {
            Renderer::ActiavatePostProcessShader("CS.glsl");
        }

        if (ImGui::Button("Remove"))
        {
            Renderer::DeactivatePostProcessShader("Post.glsl");
            Renderer::DeactivatePostProcessShader("CS.glsl");
        }
        ImGui::End();
    }

    int DevMain()
    {
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

        glfwInit();
        GLFWwindow *window = glfwCreateWindow(800, 600, "DevApp", NULL, NULL);
        glfwMakeContextCurrent(window);
        gladLoadGL();

        Renderer::Init();

        glfwSetWindowSizeCallback(window, OnResize);
        glfwMaximizeWindow(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        std::shared_ptr<Object> testObj = scene->CreateObject("Test Object");
        std::shared_ptr<Object> testObj2 = scene->CreateObject("Test2 Object");

        auto bg = testObj->AddComponent<BackgroundLayerComponent>();
        bg->GetLayer()->SetTypeSolidColor({0, 120, 0, 255});

        auto shape = testObj2->AddComponent<ShapeComponent>();
        shape->GetTransform().Position = {-2, 0, 0};

        auto shape2 = testObj2->AddComponent<ShapeComponent>();
        shape2->GetTransform().Position = {4, 0, 0};

        Renderer::ActivateScene(scene);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Renderer::BeginFrame();
            Renderer::Render();
            Renderer::EndFrame();

            RenderGui();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();

        return 0;
    }
}

int main()
{
    return WW::DevMain();
}
