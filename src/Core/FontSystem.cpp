#include <Core/FontSystem.h>
#include <glad/glad.h>
#include "Renderer/Curve.h"
#include "Transform.h"
#include "Core/Utility.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <ranges>
#include <unordered_map>
#include "App.h"
#include "freetype/internal/ftstream.h"
#include "Shader.h"


struct FontSystem::impl {
    FT_Library library{};
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
FontSystem::FontSystem() : pImpl(std::make_unique<impl>()) {
    std::cout << "FontSystem created" << std::endl;
}

std::vector<glm::vec3> FontSystem::FlattenOutlineToPoints(const FT_Outline &outline, const int nSteps ) {
    std::vector<glm::vec3> points ;
    for (int i = 0; i < outline.n_contours; ++i) {
        const int start = (i == 0) ? 0 : outline.contours[i - 1] + 1;
        const int end = outline.contours[i];

        // Linear interpolation between p0 and p1 -> quadratic bezier approximation
        // Quadratic Bezier curve = (1-t)^2 * P0 + 2(1-t)t * P1 + t^2 * P2

        for (int j = start; j <= end; ++j) {
            FT_Vector p0 = outline.points[j];
            FT_Vector p1 = outline.points[(j + 1 > end) ? start : j + 1];

            for (int step = 0; step <= nSteps; ++step) {
                float t = static_cast<float>(step) / nSteps;
                float x = (1 - t) * p0.x + t * p1.x;
                float y = (1 - t) * p0.y + t * p1.y;
                constexpr float scale = 1.0f / 64.0f; // FreeType uses 26.6 fixed point format
                points.emplace_back(x*scale, y*scale,0.0f);
            }
        }
    }
    return points ;
}

std::vector<std::vector<glm::vec3>> FontSystem::FlattenOutlineToContours(const FT_Outline &outline, const int nSteps) {
    std::vector<std::vector<glm::vec3>> contours;

    for (int i = 0; i < outline.n_contours; ++i) {
        std::vector<glm::vec3> contourPoints;
        const int start = (i == 0) ? 0 : outline.contours[i - 1] + 1;
        const int end = outline.contours[i];

        for (int j = start; j <= end; ++j) {
            FT_Vector p0 = outline.points[j];
            FT_Vector p1 = outline.points[(j + 1 > end) ? start : j + 1];

            for (int step = 0; step <= nSteps; ++step) {
                float t = static_cast<float>(step) / nSteps;
                float x = (1 - t) * p0.x + t * p1.x;
                float y = (1 - t) * p0.y + t * p1.y;
                constexpr float scale = 1.0f / 64.0f;
                contourPoints.emplace_back(x * scale, y * scale, 0.0f);
            }
        }
        contours.push_back(contourPoints);
    }
    return contours;
}

std::shared_ptr<Font> FontSystem::LoadFont(const std::string& name, const std::string& path, int fontSize) {
    auto font = std::make_shared<Font>();
    font->name = name;

    if (FT_New_Face(pImpl->library, path.c_str(), 0, &font->face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << path << std::endl;
        return nullptr;
    }

    FT_Set_Pixel_Sizes(font->face, 0, fontSize);
    // Pre-load glyphs for this font
    LoadCharacterGlyphs(font);
    fonts[name] = font;
    return font;
}

std::shared_ptr<Font> FontSystem::GetFont(const std::string& name) {
    if (fonts.contains(name)) {
        return fonts[name];
    }
    return nullptr;
}

FT_Outline FontSystem::LoadGlyphOutline(const FT_Face& face, const char character) {
    const FT_GlyphSlot slot = face->glyph;
    if (FT_Load_Char(face, character, FT_LOAD_DEFAULT)) {
        std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character: " << character << std::endl;
    }
    return slot->outline ;
}

std::vector<GameObject> FontSystem::GenerateWordMesh(const std::shared_ptr<Font>& font, const std::string& word, float characterSpacing) {
    std::vector<GameObject> objects;
    if (!font) return objects;

    float xOffset = 0.0f;
    FT_Face face = font->face;

    for (size_t i = 0; i < word.length(); ++i) {
        char character = word[i];
        // Skip spaces but add spacing
        if (character == ' ') {
            xOffset += face->size->metrics.max_advance >> 6; // Use max advance for space
            continue;
        }

        FT_Outline outline = LoadGlyphOutline(face, character);
        std::vector<std::vector<glm::vec3>> contours = FlattenOutlineToContours(outline, 20);

        if (contours.empty()) {
            continue; // Skip if no contours
        }

        Glyph characterGlyph(contours);
        const glm::vec3 position(xOffset * characterSpacing, 0.0f, 0.0f);
        Transform characterTransform(position);
        const uint32_t id = GenerateGameObjectID();
        GameObject characterObject{characterTransform, std::make_shared<Glyph>(characterGlyph)};
        objects.push_back(characterObject);

        // Advance by glyph's advance width (in font units)
        xOffset += face->glyph->advance.x >> 6;
    }

    return objects;
}


void FontSystem::LoadCharacterGlyphs(const std::shared_ptr<Font>& font) {
    if (!font) return;
    const FT_Face face = font->face;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        font->characters.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}



void  FontSystem::RenderText(const std::shared_ptr<Font>& font, Shader& shader, const std::string& text, float x, float y, const float scale, const glm::vec3 color)  {
    if (!font) return;

    shader.Use();
    shader.SetVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->textVAO);

    // Iterate through all characters
    for (char c : text) {
        if (!font->characters.contains(c)) {
            continue;
        }

        auto [TextureID, Size, Bearing, Advance] = font->characters[c];

        const float xpos = x + Bearing.x * scale;
        const float ypos = y - (Size.y - Bearing.y) * scale;

        float w = Size.x * scale;
        float h = Size.y * scale;

        // Update VBO for each character
        const float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, font->textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FontSystem::~FontSystem() {

    // Clean up character textures
    for (const auto &val: fonts | std::views::values) {
        for (auto &character: val->characters | std::views::values) {
            glDeleteTextures(1, &character.TextureID);
        }
    }
}





