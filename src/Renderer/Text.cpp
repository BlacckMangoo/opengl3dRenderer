#include "Renderer/Text.h"
#include "Core/FontSystem.h"
#include "Renderer/ResourceManager.h"
#include "Renderer/Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

void Text::Render(Renderer& renderer, const Transform& transform) {
    if ( font) {
        Shader& textShader = ResourceManager::GetShader("text");
        textShader.Use();



        // Set up orthographic projection for screen-space rendering
        const auto width = static_cast<float>(renderer.GetWindow().getWidth());
        const auto height = static_cast<float>(renderer.GetWindow().getHeight());
        const glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);



        textShader.SetMat4("projection", projection);
        textShader.SetInt("text", 0);

        // Disable depth test for 2D text overlay
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Render the text but scale to screen space from worldspace
        FontSystem::RenderText(font, textShader, content, transform.position.x* width, transform.position.y*height, scale, color);
        glEnable(GL_DEPTH_TEST);
    }
}

