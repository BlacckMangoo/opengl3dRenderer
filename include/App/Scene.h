#pragma  once
#include "Camera.h"
#include "Renderer/ModelLoader.h"
#include "GameObject.h"
#include "Editor/Editor.h"
#include "../Renderer/Geometry/ProceduralGeometryGenerator.h"
#include "Renderer/Text.h"
#include "Transform.h"


class Scene {

public:
    Scene() {
        meshes.emplace_back(std::make_shared<Mesh>(ModelLoader::LoadGLTF("../Assets/models/Cesium_Man.glb")));
        Mesh quadMesh(GenerateQuadData());
        Mesh cubeMesh(GenerateCubeData());

        meshes.emplace_back(std::make_shared<Mesh>(quadMesh));
        meshes.emplace_back(std::make_shared<Mesh>(cubeMesh));
        objects.emplace_back( Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(5.0f)), meshes[2] , "Ground Plane");



    }
    std::unique_ptr<Camera> camera = std::make_unique<Camera>();
    std::unique_ptr<Editor> editor =  std::make_unique<Editor>();
    std::vector<std::shared_ptr<Mesh>> meshes ;
    std::vector<GameObject> objects;

};
