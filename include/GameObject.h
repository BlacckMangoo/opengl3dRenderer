#pragma once
#include "Transform.h"
#include "Renderer/Model.h"
#include <memory>

class GameObject {
public:
    Transform transform;
    std::shared_ptr<Model> model;

    GameObject() = default;

    explicit GameObject(const Transform& trans, std::shared_ptr<Model> mod = nullptr)
        : transform(trans), model(std::move(mod)) {}

    // Set model for this game object
    void SetModel(std::shared_ptr<Model> mod) {
        model = std::move(mod);
    }

    // Check if game object has a valid model
    [[nodiscard]] bool HasModel() const {
        return model != nullptr;
    }
};