#include "Renderer/Model.h"
#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>


void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene, scene->mRootNode);
}

unsigned int TextureFromFile(const char *path, const std::string& directory , bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    if (unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0))
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;


        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Texture> Model::loadMaterialTextures(const aiMaterial *mat, const aiTextureType type, const std::string &typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(auto & j : textures_loaded)
        {
            if(std::strcmp(j.path.data(), str.C_Str()) == 0)
            {
                textures.push_back(j);
                skip = true;
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

void Model::ProcessNode(const aiScene *scene, const aiNode *node) {

    for ( unsigned int i = 0 ; i < node->mNumMeshes ; i++ ) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back( ProcessMesh( mesh , scene ) );
    }

    for ( unsigned int i = 0 ; i < node->mNumChildren ; i++ ) {
        ProcessNode( scene , node->mChildren[i] );
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // for each vertex process position , normal , tangent , uv
    for ( unsigned int i = 0 ; i < mesh->mNumVertices ; i++ ) {
        Vertex vertex{};
        // do some shit

        // position
       glm::vec3 vec;
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.position = vec;

        // normals

        vec.x = mesh->mNormals[i].x;
        vec.y = mesh->mNormals[i].y;
        vec.z = mesh->mNormals[i].z;
        vertex.normal = vec;

        // tangent processing

        vec.x = mesh->mTangents[i].x;
        vec.y = mesh->mTangents[i].y;
        vec.z = mesh->mTangents[i].z;
        vertex.tangent = vec;

        // if it has texture coords then process ( assimp can have 8 per vertex btw )


        if ( mesh->mTextureCoords[0]) {
            glm::vec2 vector;
            vector.x = mesh->mTextureCoords[0]->x;
            vector.y = mesh->mTextureCoords[0]->y;
            vertex.uv = vector; // uv is same as tex coords same thing basically
        }
        else {
            vertex.uv = glm::vec2(0, 0);
        }

        vertices.push_back(vertex);
    }

    // process indices

    for ( unsigned int i = 0 ; i < mesh->mNumFaces ; i++ ) {
        aiFace face = mesh->mFaces[i];
        for ( unsigned int j = 0 ; j < face.mNumIndices ; j++ ) {
            indices.push_back( face.mIndices[j] );
        }
    }

    // process material

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. roughness maps (stored as shininess in some formats)
    std::vector<Texture> roughnessMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SHININESS, "texture_roughness");
    textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

    // 5. ambient occlusion maps
    std::vector<Texture> aoMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
    textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());

    // 6. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());



    return Mesh{ vertices, indices, textures };

}




void Model::Draw(Shader &shader){
    for (const auto & mesh : meshes) {
        mesh.Draw( shader ) ;
    }

}

