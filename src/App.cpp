#include "App.h"
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Core/UiManager.h"
#include "Core/FontSystem.h"
#include "Core/Utility.h"
#include "Core/ProceduralGeometryGenerator.h"

void LoadAssets() {
    ResourceManager::LoadShader("../Assets/lit.vert", "../Assets/lit.frag", "lit");
    ResourceManager::LoadShader("../Assets/quad.vert", "../Assets/unlit.frag", "unlit");
    ResourceManager::LoadShader("../Assets/text.vert", "../Assets/text.frag", "text");
}


void Application::Init() const {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    LoadAssets();
    ImguiInit(window->get_GLFW_Window());
}


void Application::Run() {

    while ( window->isOpen()) {
        // Calculate deltaTime
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (const ImGuiIO& io = ImGui::GetIO(); !io.WantCaptureMouse) {
            currentScene->camera->ProcessInput(window->get_GLFW_Window(), deltaTime);
        }
        Render();

        // UI FRAME START
        {


        glfwPollEvents();
        ImGuiBeginFrame();

        // stuff here is updated every frame
        CameraPropertiesWindow(*currentScene->camera);
        SceneGraphWindow(*currentScene);

        //inspector ui
        for ( auto& object : currentScene->objects ) {
            if ( object.ID == currentScene->editor->selectedObjectIndex ) {
                TransformWindow(object.transform);
            }
        }

        ImGuiEndFrame();
    }
        // UI FRAME END

        glfwSwapBuffers((window->get_GLFW_Window()));
    }

}


void Application::Render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Shader& defaultShader = ResourceManager::GetShader("lit");
    for (const auto& object : currentScene->objects) {
        if (object.renderable) {
            renderer->RenderGameObject(object, defaultShader);
        }
    }

}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

