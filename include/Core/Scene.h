#pragma  once
#include "Core/Camera.h"
#include "Renderer/ModelLoader.h"
#include <memory>
#include "GameObject.h"
#include "Editor.h"
#include "FontSystem.h"
#include "Renderer/Text.h"
#include "Renderer/Curve.h"
#include "Transform.h"
#include "Utility.h"

class Scene {

public:

    Scene() {
        // Load Font
        std::shared_ptr<Font>  robotoFont = fontSystem->LoadFont("Roboto", "../Assets/Fonts/Roboto/Roboto_Condensed-Black.ttf", 48);
        std::shared_ptr<Font>  robotoDiff = fontSystem->LoadFont("Roboto2", "../Assets/Fonts/Roboto/Roboto-Italic.ttf", 48);

        models.emplace_back(std::make_shared<Model>(ModelLoader::LoadGLTF("../Assets/models/Cesium_Man.glb")));
        auto sphereText = std::make_shared<Text>("Sphere", glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, fontSystem.get(), robotoFont);
        auto sampleText = std::make_shared<Text>("sampleText" , glm::vec3(1.0f, 1.5f, 1.0f), 1.0f, fontSystem.get(), robotoDiff);
        objects.emplace_back((Transform(glm::vec3(0.0f, 0.0f, 0.0f)), models[0]));
        objects.emplace_back(Transform(glm::vec3(100.0f, 200.0f, 0.0f)), sampleText);
    }

    std::unique_ptr<Camera> camera = std::make_unique<Camera>();
    std::unique_ptr<Editor> editor =  std::make_unique<Editor>();
    std::unique_ptr<FontSystem> fontSystem = std::make_unique<FontSystem>() ;

    std::vector<std::shared_ptr<Model>> models ;
    std::vector<GameObject> objects;

};
