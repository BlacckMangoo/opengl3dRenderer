#pragma once
#include "Texture.h"
#include "../Shader.h"
#include <map>
#include <string>



class ResourceManager
{
public:
   
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string &name);
    static Shader&   GetShader(const std::string &name);
    static Texture2D LoadTexture(const char* file, bool alpha, const std::string &name);
    static Shader LoadComputeShader(const char* cShaderFile, const std::string &name);

    static Texture2D GetTexture(const std::string &name);
    static void      Clear();
private:
    ResourceManager()  = default ;
 
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
   
    static Texture2D loadTextureFromFile(const char* file, bool alpha);

    static Shader loadComputeShaderFromFile(const char* cShaderFile);


};