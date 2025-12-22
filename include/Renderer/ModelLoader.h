#pragma  once
#include <filesystem>
#include <vector>
#include <fastgltf/types.hpp>
#include "Renderer/Mesh.h"
#include "Renderer/IRenderable.h"

struct Model final : public IRenderable {
    Model() = default ;
    explicit Model( const std::vector<Mesh> &meshes ) : meshes(meshes) {};
    explicit Model (const Mesh& mesh ) {
        meshes.push_back(mesh);
    }

    std::vector<Mesh> meshes;
    void Render(Renderer &renderer, const Transform &transform) override;
};

class ModelLoader {

    public:
     explicit ModelLoader( const std::filesystem::path &path )  {
        LoadGLTF(path);
    }
    static Model LoadGLTF(  const std::filesystem::path &path ) ;
    static void ProcessNode(fastgltf::Node &node, fastgltf::Asset &asset, Model &model) ;
    static  Mesh ProcessMesh(fastgltf::Mesh &mesh, const fastgltf::Asset &asset) ;

};