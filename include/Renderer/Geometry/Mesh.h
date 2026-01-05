#pragma once
#include <vector>
#include "../IRenderable.h"
#include "Primitive.h"

class Mesh final : public IRenderable {
public:
    Mesh() = default ;
    explicit Mesh( const std::vector<Primitive> &prims ) : mesh(prims) {};
    explicit Mesh (const Primitive& prim ) {
        mesh.push_back(prim);
    }

    std::vector<Primitive> mesh;
    void Render(Renderer &renderer, const Transform &transform) override;

};
