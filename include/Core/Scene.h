#pragma  once
#include "Core/Camera.h"
#include "Renderer/Model.h"
#include <memory>

class Scene {

public:

    Scene() {
        camera = std::make_unique<Camera>();
        modelSample = std::make_shared<Model>("../Assets/backpack/backpack.obj");
    }

    std::unique_ptr<Camera> camera;
    std::shared_ptr<Model> modelSample;



};