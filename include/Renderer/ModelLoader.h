#pragma  once
#include <filesystem>
#include <vector>
#include <fastgltf/types.hpp>
#include "Renderer/Mesh.h"
#include "Renderer/IRenderable.h"

struct Model final : public IRenderable {
    std::vector<Mesh> meshes;

    void Render(Renderer& renderer, Shader& shader, const Transform& transform) override;
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