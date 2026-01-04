#pragma once
#include <Renderer/Primitive.h>
#include <Renderer/Curve.h>



inline Primitive GenerateQuadData() {
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

    Primitive quadMesh{vertices,indices,{},{}};

 return quadMesh;
}

inline Primitive GenerateCubeData() {
    const std::vector<Vertex> vertices = {
        // -Z (back)
        {{-0.5f,-0.5f,-0.5f},{0,0,-1},{1,0,0,1},{0,0}},
        {{ 0.5f,-0.5f,-0.5f},{0,0,-1},{1,0,0,1},{1,0}},
        {{ 0.5f, 0.5f,-0.5f},{0,0,-1},{1,0,0,1},{1,1}},
        {{-0.5f, 0.5f,-0.5f},{0,0,-1},{1,0,0,1},{0,1}},

        // +Z (front)
        {{-0.5f,-0.5f, 0.5f},{0,0, 1},{-1,0,0,1},{1,0}},
        {{-0.5f, 0.5f, 0.5f},{0,0, 1},{-1,0,0,1},{1,1}},
        {{ 0.5f, 0.5f, 0.5f},{0,0, 1},{-1,0,0,1},{0,1}},
        {{ 0.5f,-0.5f, 0.5f},{0,0, 1},{-1,0,0,1},{0,0}},

        // -X (left)
        {{-0.5f,-0.5f,-0.5f},{-1,0,0},{0,0, 1,1},{0,0}},
        {{-0.5f, 0.5f,-0.5f},{-1,0,0},{0,0, 1,1},{1,0}},
        {{-0.5f, 0.5f, 0.5f},{-1,0,0},{0,0, 1,1},{1,1}},
        {{-0.5f,-0.5f, 0.5f},{-1,0,0},{0,0, 1,1},{0,1}},

        // +X (right)
        {{ 0.5f,-0.5f,-0.5f},{ 1,0,0},{0,0,-1,1},{1,0}},
        {{ 0.5f,-0.5f, 0.5f},{ 1,0,0},{0,0,-1,1},{0,0}},
        {{ 0.5f, 0.5f, 0.5f},{ 1,0,0},{0,0,-1,1},{0,1}},
        {{ 0.5f, 0.5f,-0.5f},{ 1,0,0},{0,0,-1,1},{1,1}},

        // -Y (bottom)
        {{-0.5f,-0.5f,-0.5f},{0,-1,0},{1,0,0,1},{0,1}},
        {{-0.5f,-0.5f, 0.5f},{0,-1,0},{1,0,0,1},{0,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,-1,0},{1,0,0,1},{1,0}},
        {{ 0.5f,-0.5f,-0.5f},{0,-1,0},{1,0,0,1},{1,1}},

        // +Y (top)
        {{-0.5f, 0.5f,-0.5f},{0, 1,0},{1,0,0,1},{0,0}},
        {{ 0.5f, 0.5f,-0.5f},{0, 1,0},{1,0,0,1},{1,0}},
        {{ 0.5f, 0.5f, 0.5f},{0, 1,0},{1,0,0,1},{1,1}},
        {{-0.5f, 0.5f, 0.5f},{0, 1,0},{1,0,0,1},{0,1}}


    };
    const std::vector<uint32_t> indices = {
        0,  1,  2,  2,  3,  0,   // -Z
        4,  5,  6,  6,  7,  4,   // +Z
        8,  9, 10, 10, 11,  8,   // -X
       12, 13, 14, 14, 15, 12,   // +X
       16, 17, 18, 18, 19, 16,   // -Y
       20, 21, 22, 22, 23, 20    // +Y
   };


    Primitive cubeMesh{vertices,indices,{},{}};

 return cubeMesh;
}


inline Curve GenerateCurveFromPoints(const std::vector<Point>& points ) {
    return Curve{points};

}



