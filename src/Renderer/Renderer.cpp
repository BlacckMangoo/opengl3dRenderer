#include <Renderer/Renderer.h>
#include "GameObject.h"
#include "Renderer/Curve.h"
#include "Renderer/ModelLoader.h"
#include "Core/Camera.h"

void Renderer::SetupCameraUniforms(Shader& shader) const {
    shader.Use();
    const float aspectRatio = window.getAspectRatio();
    shader.SetMat4("view", camera.GetViewMatrix());
    shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    shader.SetVec3("viewPos", camera.GetPosition());
}

void Renderer::SetupModelMatrix(Shader& shader, const Transform& transform) {
    const glm::mat4 modelMatrix = transform.GetTransformMatrix();
    shader.SetMat4("model", modelMatrix);
}

void Renderer::RenderGameObject(const GameObject& gameObject, Shader& shader) {
    if (gameObject.renderable) {

        gameObject.renderable->Render(*this, shader, gameObject.transform);
    }
}

void Renderer::DrawMesh(const Mesh& mesh, Shader& shader, const RenderCommand& command) {
    shader.Use();
    glPolygonMode(GL_FRONT_AND_BACK, command.GetGLPolygonMode());
    glBindVertexArray(mesh.VAO);
    glDrawElements(command.GetGLPrimitiveType(), mesh.indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::DrawCurve(const Curve& curve) {
    glBindVertexArray(curve.VAO);
    glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(curve.points.size()));
    glBindVertexArray(0);
}

// ============ IRenderable Implementations ============


void Model::Render(Renderer& renderer, Shader& shader, const Transform& transform) {
    renderer.SetupCameraUniforms(shader);
    Renderer::SetupModelMatrix(shader, transform);

    const glm::mat4 modelMatrix = transform.GetTransformMatrix();
    const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    shader.SetMat3("normalMatrix", normalMatrix);
    shader.SetVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    for (const auto& mesh : meshes) {
        Renderer::DrawMesh(mesh, shader, RenderCommand::Default());
    }
}

