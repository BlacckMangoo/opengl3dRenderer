#include <Core/FontLoader.h>
#include <ft2build.h>
#include "Renderer/Mesh.h"
#include FT_FREETYPE_H
#include <iostream>
#include <filesystem>
#include <ranges>
#include <unordered_map>
#include "freetype/internal/ftstream.h"


struct FontLoader::impl {
    FT_Library library;
    std::unordered_map<std::string, FT_Face> faces;

    impl() {
        if (FT_Init_FreeType(&library)) {
            std::cerr << "ERROR::FREETYPE: Failed to initialize FreeType library" << std::endl;
        }
    }

    ~impl() {
        for (const auto &val: faces | std::views::values) {
            FT_Done_Face(val);
        }
        FT_Done_FreeType(library);
    }
};

// Constructor implementation
FontLoader::FontLoader() : pImpl(std::make_unique<impl>()) {
    std::cout << "FontLoader created" << std::endl;

}

std::vector<glm::vec3> FontLoader::FlattenOutlineToPoints(const FT_Outline &outline, const int nSteps ) {
    std::vector<glm::vec3> points ;
    for (int i = 0; i < outline.n_contours; ++i) {
        int start = (i == 0) ? 0 : outline.contours[i - 1] + 1;
        int end = outline.contours[i];

        // Linear interpolation between p0 and p1 -> quadratic bezier approximation
        // Quadratic Bezier curve = (1-t)^2 * P0 + 2(1-t)t * P1 + t^2 * P2

        for (int j = start; j <= end; ++j) {
            FT_Vector p0 = outline.points[j];
            FT_Vector p1 = outline.points[(j + 1 > end) ? start : j + 1];

            for (int step = 0; step <= nSteps; ++step) {
                float t = static_cast<float>(step) / nSteps;
                float x = (1 - t) * p0.x + t * p1.x;
                float y = (1 - t) * p0.y + t * p1.y;
                points.emplace_back(x/64, y/64,0.0f);
            }
        }
    }
    return points ;
}

 FT_Face FontLoader::LoadFont(const char* fontPath , const int fontSize ) const {
    FT_Face face;
    if (FT_New_Face(pImpl->library, fontPath, 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontPath << std::endl;

    }
    const std::string fontName = std::filesystem::path(fontPath).filename().string();
    pImpl->faces[fontName] = face;
    std::cout << "Font loaded successfully: " << fontName << std::endl;
    // Set font size
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    return face ;
}

FT_Outline FontLoader::LoadGlyphOutline(const FT_Face& face, const char character) {

    const FT_GlyphSlot slot = face->glyph;
    if (FT_Load_Char(face, character, FT_LOAD_DEFAULT)) {
        std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character: " << character << std::endl;
    }
    return slot->outline ;
}


FontLoader::~FontLoader() = default;
