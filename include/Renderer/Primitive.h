#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>
#include "Shader.h"

struct Vertex {

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 tangent; // x,y,z = tangent , w = handedness ( bittangent)
    glm::vec2 uv  ;

};



struct Texture {
    GLuint id ;
    GLuint width , height;
    GLenum format  ;
    bool hasMipMaps  ;
};



struct Material {
    Texture* albedo = nullptr ;
    Texture* normalMap = nullptr ;
    Texture* metallicRoughnessMap = nullptr ;
    Texture* aoMap = nullptr ;
    Texture* emissiveMap = nullptr ;
    glm::vec4 baseColorFactor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    float metallicFactor     = 0.0f;
    float roughnessFactor    = 0.5f;
    float aoFactor           = 1.0f;
    glm::vec3 emissiveFactor = glm::vec3(0.0f);

};


class Primitive {
public:
    unsigned int VAO{} , VBO{} , EBO{} ;
    int indexCount{} ;

    std::string name ;

    std::vector<Vertex> vertices ;
    std::vector<unsigned int> indices ;
    std::vector<Texture> textures ;
    Material material  ;

    Primitive() = default ;

    Primitive(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures ,const Material &material = {}) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this ->material = material ;
        MeshDataInitialise();
        this->indexCount = static_cast<int>(indices.size());
    } ;

    void MeshDataInitialise() ;
     };