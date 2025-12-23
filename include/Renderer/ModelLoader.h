#pragma  once
#include <filesystem>
#include <vector>
#include <fastgltf/types.hpp>
#include "Renderer/Primitive.h"
#include "Renderer/IRenderable.h"

struct Mesh final : public IRenderable {
    Mesh() = default ;
    explicit Mesh( const std::vector<Primitive> &prims ) : mesh(prims) {};
    explicit Mesh (const Primitive& prim ) {
        mesh.push_back(prim);
    }

    std::vector<Primitive> mesh;
    void Render(Renderer &renderer, const Transform &transform) override;
};

class ModelLoader {

    public:
     explicit ModelLoader( const std::filesystem::path &path )  {
        LoadGLTF(path);
    }
    static Mesh LoadGLTF(  const std::filesystem::path &path ) ;
    static void ProcessNode(fastgltf::Node &node, fastgltf::Asset &asset, Mesh &model) ;
    static  Primitive ProcessPrimtives(fastgltf::Mesh &mesh, const fastgltf::Asset &asset) ;

};