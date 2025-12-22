#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ResourceManager.h"
#include "Window.h"
#include "Transform.h"

// Forward declarations
class FontSystem;
class Scene;
class GameObject;
class Mesh;
class Curve;
class Text;
class Camera;

enum PrimitiveType {
    TRIANGLES = 0,
    LINES = 1,
    POINTS = 2
};

enum FillMode {
    FILL = 0,
    LINE = 1,
    POINT = 2
};

struct RenderCommand {
    PrimitiveType primitiveType = TRIANGLES;
    FillMode fillMode = FILL;

    [[nodiscard]] unsigned int GetGLPrimitiveType() const {
        switch(primitiveType) {
            case TRIANGLES: return GL_TRIANGLES;
            case LINES: return GL_LINES;
            case POINTS: return GL_POINT;
            default: return GL_TRIANGLES;
        }
    }

    [[nodiscard]] unsigned int GetGLPolygonMode() const {
        switch(fillMode) {
            case FILL: return GL_FILL;
            case LINE: return GL_LINE;
            case POINT: return GL_POINT;
            default: return GL_FILL;
        }
    }

    static RenderCommand Default() { return {TRIANGLES, FILL}; }
    static RenderCommand Wireframe() { return {TRIANGLES, LINE}; }
    static RenderCommand PointCloud() { return {POINTS, POINT}; }
};

class Renderer {
public:
    Renderer(Camera& camera, Window& window) : camera(camera), window(window) {
        InitScreenQuad();
        InitFramebuffer(window.getWidth(), window.getHeight());
    }
    void RenderGameObject(const GameObject &gameObject);
    void SetupCameraUniforms(Shader& shader) const;

    static void SetupModelMatrix(Shader& shader, const Transform& transform);
    static void DrawMesh(const Mesh& mesh, Shader& shader, const RenderCommand& command);

    static void DrawCurve(const Curve& curve);

    void InitScreenQuad();
    void BeginScenePass() const;
    void EndScenePass() const;
    void InitFramebuffer(int width, int height);
    [[nodiscard]] Camera& GetCamera() const { return camera; }
    [[nodiscard]] Window& GetWindow() const { return window; }

    Shader& screenShader = ResourceManager::GetShader("screen");

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    unsigned int fbo = 0; // Default Framebuffer Object
    unsigned int colorTexture = 0;
    unsigned int depthTexture = 0;
    unsigned int rbo = 0;


private:
    Camera& camera;
    Window& window;
};



