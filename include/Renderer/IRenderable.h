#pragma once

// Forward declarations
class Renderer;
class Shader;
class Scene;
struct Transform;

// Interface for all renderable types
struct IRenderable {
    virtual ~IRenderable() = default;
    virtual void Render(Renderer &renderer, const Transform &transform) = 0;
};

