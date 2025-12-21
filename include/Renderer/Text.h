#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <utility>
#include "IRenderable.h"

class FontSystem;
struct Font;

// Text renderable type - holds text content for bitmap rendering
class Text final : public IRenderable {
public:
    std::string content;
    glm::vec3 color = glm::vec3(1.0f);
    float scale = 1.0f;
    std::shared_ptr<Font> font;

    Text(std::string  text, const glm::vec3 color, const float scale, std::shared_ptr<Font> f)
        : content(std::move(text)), color(color), scale(scale), font(std::move(f)) {}

    void Render(Renderer& renderer, Shader& shader, const Transform& transform) override;
};

