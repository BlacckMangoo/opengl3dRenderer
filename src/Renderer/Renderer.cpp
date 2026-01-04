#include <Renderer/Renderer.h>
#include "GameObject.h"
#include "Renderer/Curve.h"
#include "Renderer/ModelLoader.h"
#include "Core/Camera.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
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





void Renderer::InitScreenQuad()
{
    constexpr float quadVertices[] = {
        // pos      // uv
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void *>(0)); // 4 floats per vertex, position starts at offset 0

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float))); // uv starts at offset 2

    glBindVertexArray(0);
}
void Renderer::InitFramebuffer(const int width, const int height)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Color texture
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,width, height,0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        colorTexture,
        0
    );


    // Depth renderbuffer
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        width, height
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        rbo
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer incomplete\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::BeginScenePass() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::RenderGameObject(const GameObject& gameObject) {
    if (gameObject.renderable) {
        gameObject.renderable->Render(*this, gameObject.transform);
    }
}

void Renderer::DrawPrimitive(const Primitive& prim, Shader& shader, const RenderCommand& command) {
    shader.Use();
    // Set material uniforms constants

    shader.SetFloat("u_AoFactor" , prim.material.aoFactor);
    shader.SetFloat("u_MetallicFactor" , prim.material.metallicFactor);
    shader.SetFloat("u_RoughnessFactor" , prim.material.roughnessFactor);
    shader.SetVector4f("u_BaseColorFactor" , prim.material.baseColorFactor);
    shader.SetVec3("u_EmissiveFactor" , prim.material.emissiveFactor);

    glPolygonMode(GL_FRONT_AND_BACK, command.GetGLPolygonMode());
    glBindVertexArray(prim.VAO);
    glDrawElements(command.GetGLPrimitiveType(), prim.indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}

void Renderer::DrawCurve(const Curve& curve) {
    glBindVertexArray(curve.VAO);
    glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(curve.points.size()));
    glBindVertexArray(0);
}

void Mesh::Render(Renderer &renderer, const Transform &transform) {
    Shader& shader = ResourceManager::GetShader("lit");
    renderer.SetupCameraUniforms(shader);
    Renderer::SetupModelMatrix(shader, transform);

    // material uniforms

    const glm::mat4 modelMatrix = transform.GetTransformMatrix();
    const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    shader.SetMat3("normalMatrix", normalMatrix);
    shader.SetVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    for (const auto& prim : mesh) {
        Renderer::DrawPrimitive(prim, shader, RenderCommand::Default());
    }
}



void Renderer::EndScenePass() const  {

    // binds back to default framebuffer and draws the quad with scene's texture

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    screenShader.Use();
    screenShader.SetInt("screenTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
