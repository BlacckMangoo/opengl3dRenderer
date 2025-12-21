#pragma once
#include <Renderer/Mesh.h>
// a list of inline header functions to generate procedural meshes and vertices

inline Mesh GenerateQuadData() {
    // Define vertices for a quad
    const std::vector<Vertex> vertices = {
        // positions          // normals         // tangents           // uvs
        {{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
    };

    // Define indices for two triangles that make up the quad
    const std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    Mesh quadMesh{vertices,indices,{},{}};

 return quadMesh;
}

