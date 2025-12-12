#pragma once
#include "glm/glm.hpp"

#include <string>
#include <vector>

#include "Shader.h"

struct Vertex {

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 uv  ;

};

struct Texture {
    unsigned int id ;
    std::string type  ;
    std::string path  ;
};




class Mesh {

public:


    unsigned int VAO{} , VBO{} , EBO{} ;
    int indexCount{} ;


    std::vector<Vertex> vertices ;
    std::vector<unsigned int> indices ;
    std::vector<Texture> textures ;


    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->indexCount = static_cast<int>(indices.size());
        MeshDataInitialise();
    } ;

    void MeshDataInitialise() ;

    void Draw(Shader &shader) const;



};