#pragma  once
#include "Core/Camera.h"
#include "Renderer/ModelLoader.h"
#include "GameObject.h"
#include "Editor.h"
#include "FontSystem.h"
#include "ProceduralGeometryGenerator.h"
#include "Renderer/Text.h"
#include "Transform.h"


class Scene {

public:
    Scene() {
        models.emplace_back(std::make_shared<Model>(ModelLoader::LoadGLTF("../Assets/models/Cesium_Man.glb")));
        objects.emplace_back((Transform(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(-1.8f,0.0f,4.5f),glm::vec3(1.0f,1.0f,1.0f)),models[0] ));

    }
    std::unique_ptr<Camera> camera = std::make_unique<Camera>();
    std::unique_ptr<Editor> editor =  std::make_unique<Editor>();
    std::vector<std::shared_ptr<Model>> models ;
    std::vector<GameObject> objects;

};
