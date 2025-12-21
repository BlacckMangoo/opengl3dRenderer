#include <Renderer/Curve.h>
#include <glad/glad.h>
#include "Shader.h"
#include "Transform.h"
#include "Renderer/ResourceManager.h"
#include "Renderer/Renderer.h"

void Curve::CurveDataInitialise() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), static_cast<void*>(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Curve::Render(Renderer& renderer, Shader& shader, const Transform& transform) {
    // Curves use unlit shader
    Shader& unlitShader = ResourceManager::GetShader("unlit");
    renderer.SetupCameraUniforms(unlitShader);
    renderer.SetupModelMatrix(unlitShader, transform);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, points.size());
    glBindVertexArray(0);
}

void Glyph::Render(Renderer& renderer, Shader& shader, const Transform& transform) {
    // Glyphs use unlit shader for wireframe text
    Shader& unlitShader = ResourceManager::GetShader("unlit");
    renderer.SetupCameraUniforms(unlitShader);
    renderer.SetupModelMatrix(unlitShader, transform);

    for (auto& curve : curves) {
        renderer.DrawCurve(*curve);
    }
}
