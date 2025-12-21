#pragma once
#include <memory>

#include "freetype/freetype.h"
#include "freetype/ftimage.h"

class FontLoader {

public:
    FontLoader();
    ~FontLoader();

    static std::vector<glm::vec3>  FlattenOutlineToPoints(const FT_Outline& outline, const int nSteps = 10) ;
    FT_Face LoadFont(const char *fontPath, int fontSize) const;

    static FT_Outline LoadGlyphOutline(const FT_Face &face, char character);
private :
    struct impl;
     std::unique_ptr<impl> pImpl;
};
