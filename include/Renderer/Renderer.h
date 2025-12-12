#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Scene.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Window.h"

class Renderer {
public :

    Renderer( Camera& camera , Window& window  ) : camera( camera), window(window) {
    }


    void RenderGameObject(const GameObject& gameObject, Shader& shader) const {
        if (!gameObject.HasModel()) {
            return;
        }


        shader.Use();

        // Set camera uniforms
        const float aspectRatio = window.getAspectRatio();
        shader.SetMat4("view", camera.GetViewMatrix());
        shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
        shader.SetVec3("viewPos", camera.GetPosition());

        glm::mat4 modelMatrix = gameObject.transform.GetTransform();
        shader.SetMat4("model", modelMatrix);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
        shader.SetMat3("normalMatrix", normalMatrix);

        // Set lighting uniforms
        shader.SetVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
        shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetFloat("shininess", 32.0f);

        // Draw all meshes in the model
        for (const auto& mesh : gameObject.model->meshes) {
            mesh.Draw(shader);
        }
    }

private:
    Camera& camera ;
    Window & window ;


};