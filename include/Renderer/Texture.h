
#pragma once
#include "glad/glad.h"
#include <string>
class Texture2D
{
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particular texture
      GLuint ID{};
    // texture image dimensions
      GLint Width, Height; // width and height of loaded image in pixels
    // texture Format
      GLint Internal_Format; // format of texture object
      GLuint Image_Format; // format of loaded image
    // texture configuration
      GLint Wrap_S; // wrapping mode on S axis
      GLint Wrap_T; // wrapping mode on T axis
      GLint Filter_Min; // filtering mode if texture pixels < screen pixels
      GLint Filter_Max; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void Generate(GLint width, GLint height, const unsigned char *data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;

};