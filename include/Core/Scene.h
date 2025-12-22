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
        // Load Font
        std::shared_ptr<Font>  robotoFont = fontSystem->LoadFont("Roboto", "../Assets/Fonts/Roboto/Roboto_Condensed-Black.ttf", 42);
        models.emplace_back(std::make_shared<Model>(ModelLoader::LoadGLTF("../Assets/models/Cesium_Man.glb")));
        models.emplace_back(std::make_shared<Model>(GenerateQuadData()));
        auto sampleText = std::make_shared<Text>("TextRendering" , glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, robotoFont);
        objects.emplace_back((Transform(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(-1.8f,0.0f,4.5f),glm::vec3(1.0f,1.0f,1.0f)),models[0] ));
        objects.emplace_back(Transform(glm::vec3(0.1f, 0.2f, 0.0f)), sampleText);

    }

    std::unique_ptr<Camera> camera = std::make_unique<Camera>();
    std::unique_ptr<Editor> editor =  std::make_unique<Editor>();
    std::unique_ptr<FontSystem> fontSystem = std::make_unique<FontSystem>() ;
    std::vector<std::shared_ptr<Model>> models ;
    std::vector<GameObject> objects;

};
