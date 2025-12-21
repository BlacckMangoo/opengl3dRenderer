#pragma once
#include "Transform.h"
#include <memory>
#include <Core/Utility.h>
#include "Renderer/IRenderable.h"
class GameObject {
public:
    Transform transform;
    std::shared_ptr<IRenderable> renderable = nullptr;
    uint32_t ID = 0;

    GameObject(){
        ID = GenerateGameObjectID();
    };

    explicit GameObject(const Transform& trans, std::shared_ptr<IRenderable> rend = nullptr)
        : transform(trans), renderable(std::move(rend)){     ID = GenerateGameObjectID();
}

    explicit GameObject(std::shared_ptr<IRenderable> rend)
        : renderable(std::move(rend)) {
        ID = GenerateGameObjectID();
        }

};