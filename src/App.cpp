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
            ImGui::Begin("Inspector");
        if (currentScene->editor->selectedObjectIndex < currentScene->objects.size()) {
            TransformWindow(currentScene->objects[currentScene->editor->selectedObjectIndex].transform);
            // material window if renderable is a mesh
            if (auto& selectedObject = currentScene->objects[currentScene->editor->selectedObjectIndex]; selectedObject.renderable) {
                if (const auto meshPtr = std::dynamic_pointer_cast<Model>(selectedObject.renderable)) {
                    MaterialPropsWindow(meshPtr->meshes[0].material);
                }
            }
            ImGui::End();

        ImGuiEndFrame();
    };
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
            renderer->RenderGameObject(object);
        }
    }

}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
