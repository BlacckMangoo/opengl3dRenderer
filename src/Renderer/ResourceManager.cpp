
#include <glad/glad.h>
#include "Renderer/ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"
#include <filesystem>
#include <ranges>
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const std::filesystem::path &vShaderFile, const std::filesystem::path fShaderFile, const std::string &name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::GetShader(const std::string &name)
{
  return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const std::filesystem::path &file, bool alpha, const std::string &name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string &name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (const auto &val: Shaders | std::views::values)
        glDeleteProgram(val.ID);
    // (properly) delete all textures
    for (const auto &val: Textures | std::views::values)
        glDeleteTextures(1, &val.ID);
}

Shader ResourceManager::loadShaderFromFile(const std::filesystem::path &vShaderFile, const std::filesystem::path &fShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);

        // Check if files opened successfully
        if (!vertexShaderFile.is_open()) {
            std::cerr << "ERROR::SHADER: Failed to open vertex shader file: " << vShaderFile << std::endl;
            return {}; // Return empty shader
        }
        if (!fragmentShaderFile.is_open()) {
            std::cerr << "ERROR::SHADER: Failed to open fragment shader file: " << fShaderFile << std::endl;
            return {}; // Return empty shader
        }

        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // Check if we actually read anything
        if (vertexCode.empty()) {
            std::cerr << "ERROR::SHADER: Vertex shader file is empty: " << vShaderFile << std::endl;
        }
        if (fragmentCode.empty()) {
            std::cerr << "ERROR::SHADER: Fragment shader file is empty: " << fShaderFile << std::endl;
        }


    }
    catch (std::exception& e)
    {
        std::cout << "ERROR::SHADER: Exception while reading shader files: " << e.what() << " - " << vShaderFile << ", " << fShaderFile << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode);

    // Basic sanity: if compilation didn't set ID, warn
    if (shader.ID == 0) {
        std::cerr << "ResourceManager::loadShaderFromFile - compiled shader has ID 0 (compile/link likely failed): "
                  << vShaderFile << ", " << fShaderFile << std::endl;
    }
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const std::filesystem::path &file, const bool alpha)
{
    // create texture object
    Texture2D texture;
    const char *fileCStr = file.string().c_str();
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(fileCStr, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}


Texture2D ResourceManager::GenerateSolidColorTexture(unsigned char r, unsigned char g, unsigned char b, unsigned char a, const std::string &name)
{
    Texture2D texture;
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;

    unsigned char colorData[4] = { r, g, b, a };
    texture.Generate(1, 1, colorData);
    Textures[name] = texture;
    return texture;
}

Shader ResourceManager::LoadComputeShader(const std::filesystem::path &cShaderFile, const std::string &name)
{
    Shaders[name] = loadComputeShaderFromFile(cShaderFile);
    return Shaders[name];
}

Shader ResourceManager::loadComputeShaderFromFile(const std::filesystem::path &cShaderFile)
{
    std::string computeCode;
    try
    {
        std::ifstream computeShaderFile(cShaderFile);
        std::stringstream cShaderStream;
        cShaderStream << computeShaderFile.rdbuf();
        computeShaderFile.close();
        computeCode = cShaderStream.str();
    }
    catch (std::exception &e)
    {
        std::cout << "ERROR::SHADER: Failed to read compute shader file: " << cShaderFile << std::endl;
    }

    const char* cShaderCode = computeCode.c_str();
    Shader shader;
    shader.CompileCompute(cShaderCode);

    if (shader.ID == 0) {
        std::cerr << "ResourceManager::loadComputeShaderFromFile - compiled compute shader has ID 0 (compile/link likely failed): "
                  << cShaderFile << std::endl;
    }
    return shader;
}
