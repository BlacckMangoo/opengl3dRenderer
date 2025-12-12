#pragma once
#include <vector>
#include "Renderer/Mesh.h"
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"


// -> load model
// process nodes and meshes
// store meshes and textures

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {

public :
     std::vector<Texture> textures_loaded ;
     std::vector<Mesh> meshes;
     std::string directory ;

    void Draw(Shader &shader);

    bool gammaCorrection ;
    explicit Model( const std::string& path , bool gamma = false ) : gammaCorrection( gamma ) {
        LoadModel( path ) ;
    }

    void LoadModel( const std::string& path ) ;
    void ProcessNode(  const  aiScene *scene , const aiNode *node ) ;
     std::vector <Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string &typeName);

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene) ;



private:


};