#pragma  once
#include <filesystem>
#include <Renderer/Mesh.h>
#include <fastgltf/types.hpp>
#include "Renderer/Primitive.h"

class ModelLoader {

    public:
     explicit ModelLoader( const std::filesystem::path &path )  {
        LoadGLTF(path);
    }
    static Mesh LoadGLTF(  const std::filesystem::path &path ) ;
    static void ProcessNode(fastgltf::Node &node, fastgltf::Asset &asset, Mesh &model) ;
    static  Primitive ProcessPrimtives(fastgltf::Mesh &mesh, const fastgltf::Asset &asset) ;

};