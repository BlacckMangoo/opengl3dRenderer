#pragma once
#include "Resources/ResourceManager.h"
#include "Window.h"
#include "Transform.h"

// Forward declarations
class FontSystem;
class Scene;
class GameObject;
class Primitive;
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
    static void DrawPrimitive(const Primitive& prim, Shader& shader, const RenderCommand& command);

    void InitScreenQuad() ;
    void InitFramebuffer(int width, int height) ;

    void BeginScenePass() const;
    void EndScenePass() const;

    [[nodiscard]] Camera& GetCamera() const { return camera; }
    [[nodiscard]] Window& GetWindow() const { return window; }


    GLuint quadVAO = 0;
    GLuint quadVBO = 0;
    GLuint fbo = 0; // Default Framebuffer Object
    GLuint colorTexture = 0;
    GLuint depthTexture = 0;
    GLuint rbo = 0;


private:
    Camera& camera;
    Window& window;
};



