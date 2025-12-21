#pragma once
#include <memory>
#include "Renderer/Renderer.h"
#include "Core/Scene.h"
class Application {

public :
    Application()  {
    window = std::make_unique<Window>(1920, 1080, "Game");
    currentScene = std::make_unique<Scene>();
    renderer = std::make_unique<Renderer>(*currentScene->camera, *window);

        // Setup mouse callbacks for camera
        Camera::SetupMouseCallbacks(window->get_GLFW_Window(), currentScene->camera.get());
        Init();

    }

    ~Application() ;
     void Init() const;
    void Render() const;
    void Run();

private:

    std::unique_ptr<Scene> currentScene  ;  // holds camera , meshes , lights etc
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;

    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;


};