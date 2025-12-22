#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "IRenderable.h"

#define Point glm::vec3

class Curve final : public IRenderable {
public:
    unsigned int VAO{}, VBO{};
    std::vector<Point> points;

    void CurveDataInitialise();
    explicit Curve(const std::vector<Point>& pts) {
        points = pts;
        CurveDataInitialise();
    }
    void Render(Renderer &renderer, const Transform &transform) override;
};

// A glyph/character is made of multiple curves (contours)
class Glyph final : public IRenderable {
public:
    std::vector<std::shared_ptr<Curve>> curves;

    explicit Glyph(const std::vector<std::vector<Point>>& contours) {
        for (const auto& contour : contours) {
            curves.push_back(std::make_shared<Curve>(contour));
        }
    }

    void Render(Renderer &renderer, const Transform &transform) override;
};


