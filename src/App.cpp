#include "App.h"
#include "Renderer/ResourceManager.h"


void Application::Init() const {
    glEnable(GL_DEPTH_TEST);
    ResourceManager::LoadShader("../Assets/lit.vert", "../Assets/lit.frag", nullptr, "lit");
    Camera::SetupMouseCallbacks(window->get_GLFW_Window(), scene->camera.get());
}

void Application::Run() {

    while ( window->isOpen()) {

        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Update();
        Render();
        glfwPollEvents();
        glfwSwapBuffers((window->get_GLFW_Window()));
    }

}

void Application::Update() const {
    scene->camera->ProcessInput(window->get_GLFW_Window(), deltaTime);
}

void Application::Render() const {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create a GameObject with a transform and model
    const Transform transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    const GameObject gameObject(transform, scene->modelSample);
    // Render the game object with lit shader
    renderer->RenderGameObject(gameObject, ResourceManager::GetShader("lit"));

    glBindVertexArray(0);

};


