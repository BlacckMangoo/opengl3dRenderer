#include "App.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Core/UiManager.h"
#include "Core/ProceduralGeometryGenerator.h"
#include "glmDebugger.h"

void LoadAssets() {
    ResourceManager::LoadShader("../Assets/lit.vert", "../Assets/lit.frag", "lit");
    ResourceManager::LoadShader("../Assets/quad.vert", "../Assets/unlit.frag", "unlit");
    ResourceManager::LoadShader("../Assets/text.vert", "../Assets/text.frag", "text");
    ResourceManager::LoadShader("../Assets/quad.vert", "../Assets/quad.frag", "screen");
}
void Application::Init() const {
    LoadAssets();
    ImguiInit(window->get_GLFW_Window());

}

void Application::Run() {

    while ( window->isOpen()) {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (const ImGuiIO& io = ImGui::GetIO(); !io.WantCaptureMouse) {
            currentScene->camera->ProcessInput(window->get_GLFW_Window(), deltaTime);
        }
        Render();
        glfwPollEvents();
        ImGuiBeginFrame();
        CameraPropertiesWindow(*currentScene->camera);
        SceneGraphWindow(*currentScene);
        ImGuiEndFrame();

        glfwSwapBuffers((window->get_GLFW_Window()));
    }

}


void Application::Render() const {

    renderer->BeginScenePass();
    for (const auto& object : currentScene->objects) {
        renderer ->RenderGameObject(object);
    }
    renderer->EndScenePass();


}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
