#pragma once
#include <memory>
#include <vector>
#include <string>
#include "freetype/freetype.h"
#include "freetype/ftimage.h"
#include "GameObject.h"
#include <map>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    FT_Pos Advance;    // Offset to advance to next glyph
};

struct Font {

    Font() {
        glGenVertexArrays(1, &textVAO);
        glGenBuffers(1, &textVBO);

        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);

        // Allocate buffer for dynamic text (will be updated per draw call)
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

        // Position + TexCoord attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    unsigned int textVAO = 0;
    unsigned int textVBO = 0;

    FT_Face face{};
    std::map<char, Character> characters;
    std::string name;
};

class FontSystem {

public:
    FontSystem();
    ~FontSystem();

    static std::vector<glm::vec3>  FlattenOutlineToPoints(const FT_Outline& outline, int nSteps = 10) ;
    static std::vector<std::vector<glm::vec3>> FlattenOutlineToContours(const FT_Outline& outline, int nSteps = 10);
    std::shared_ptr<Font> LoadFont(const std::string& name, const std::string& path, int fontSize);
    std::shared_ptr<Font> GetFont(const std::string& name);

    static FT_Outline LoadGlyphOutline(const FT_Face &face, char character);
    static std::vector<GameObject> GenerateWordMesh(const std::shared_ptr<Font>& font, const std::string& word, float characterSpacing = 1.2f); // creates each character as a separate GameObject
    static void RenderText(const std::shared_ptr<Font>& font, class Shader& shader, const std::string& text, float x, float y, float scale, glm::vec3 color) ;

private :
    struct impl;
    std::unique_ptr<impl> pImpl;
    std::map<std::string, std::shared_ptr<Font>> fonts;
    static  void LoadCharacterGlyphs(const std::shared_ptr<Font>& font);
};
