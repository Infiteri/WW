#include "Renderer.h"
#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <windows.h>
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
    }

    int DevMain()
    {
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

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Renderer::Render();

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
