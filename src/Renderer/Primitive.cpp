#include "../../include/Renderer/Geometry/Primitive.h"
#include <glad/glad.h>


void Primitive::MeshDataInitialise() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    const auto byteSize = vertices.size() * sizeof(Vertex);
    assert(byteSize <= static_cast<size_t>(std::numeric_limits<GLsizeiptr>::max()));

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(byteSize),
        vertices.data(),
        GL_STATIC_DRAW
    );


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    const auto indexByteSize = indices.size() * sizeof(unsigned int);
    // Ensure the size fits within GLsizeiptr limits , if overflow occurs it may wrap around and cause garbage data to be uploaded

    assert(indexByteSize <= static_cast<size_t>(std::numeric_limits<GLsizeiptr>::max()));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,static_cast<GLsizeiptr>(indexByteSize), &indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));
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


