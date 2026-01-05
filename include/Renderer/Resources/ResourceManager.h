#pragma once
#include "Texture.h"
#include "Shader.h"
#include <map>
#include <string>
#include <filesystem>
#include "Renderer/Geometry/Mesh.h"


class ResourceManager
{
public:
   
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string ,Mesh >  Meshes ;


    static Shader       LoadShader(const std::filesystem::path &vShaderFile, const std::filesystem::path& fShaderFile, const std::string &name);
    static Shader&      GetShader(const std::string &name);
    static Texture2D    LoadTexture(const std::filesystem::path &file, bool alpha, const std::string &name);
    static Shader       LoadComputeShader(const std::filesystem::path &cShaderFile, const std::string &name);
    static Texture2D    GetTexture(const std::string &name);
    static void         Clear();
    private:
    ResourceManager()  = default ;
 
    static Shader       loadShaderFromFile(const std::filesystem::path &vShaderFile, const std::filesystem::path &fShaderFile);
    static Texture2D    loadTextureFromFile(const std::filesystem::path &file, bool alpha);
    static Shader       loadComputeShaderFromFile(const std::filesystem::path &cShaderFile);

};
