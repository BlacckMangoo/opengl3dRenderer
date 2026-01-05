#pragma once
#include "Transform.h"
#include <memory>
#include <utility>
#include <Core/Id.h>
#include "Renderer/IRenderable.h"
class GameObject {
public:
    Transform transform ;
    std::shared_ptr<IRenderable> renderable = nullptr;
    uint32_t ID = 0;
    std::string name;

    GameObject(){
        ID = GenerateGameObjectID();
    };
    //we use move for name to avoid unnecessary copies of name string and shared_ptr for renderable to manage memory automatically
    explicit GameObject( const Transform& trans, std::shared_ptr<IRenderable> rend = nullptr , std::string n = "")
        : transform(trans), renderable(std::move(rend)){ transform = trans ;  ID = GenerateGameObjectID() ; name = std::move(n);
}

};