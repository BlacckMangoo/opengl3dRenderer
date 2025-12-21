#include "Renderer/Mesh.h"
#include <glad/glad.h>
#include <string>
#include "Renderer/ResourceManager.h"

void Mesh::MeshDataInitialise() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(0));
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
    // Vertex Tangents
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, tangent)));
    // Vertex Texture Coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, uv)));

    glBindVertexArray(0);


}

void BindTexture(const int unit, const Texture* texture, const std::string& uniformName, Shader& shader) {
    if (texture) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        shader.SetInt(uniformName.c_str(), unit);
    }
}


 void Mesh::Draw(Shader &shader ) const {

    shader.Use();
    // Texture samplers
    // uniform sampler2D u_BaseColorMap;        // slot 0
    // uniform sampler2D u_MetallicRoughnessMap;// slot 1
    // uniform sampler2D u_NormalMap;           // slot 2
    // uniform sampler2D u_OcclusionMap;         // slot 3
    // uniform sampler2D u_EmissiveMap;          // slot 4



    // draw mesh
    glPolygonMode(GL_FRONT_FACE , GL_FILL);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);


}
