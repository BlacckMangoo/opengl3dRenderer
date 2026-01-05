#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>
#include "../Resources/Shader.h"

struct Vertex {

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 tangent; // x,y,z = tangent , w = handedness ( bittangent)
    glm::vec2 uv  ;

};


struct Material {

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

    std::vector<Vertex> vertices ;
    std::vector<unsigned int> indices ;
    Material material  ;

    Primitive(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices ,const Material &material = {}) {
        this->vertices = vertices;
        this->indices = indices;
        this ->material = material ;
        MeshDataInitialise();
        this->indexCount = static_cast<int>(indices.size());
    } ;

    void MeshDataInitialise() ;
     };