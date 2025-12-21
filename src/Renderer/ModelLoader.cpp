#include <Renderer/ModelLoader.h>
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <iostream>


Model ModelLoader::LoadGLTF( const std::filesystem::path &path) {
    // Parse the glTF file and get the constructed asset
    {
        Model model;


        fastgltf::Parser parser{};

        constexpr auto gltfOptions =
            fastgltf::Options::DontRequireValidAssetMember |
            fastgltf::Options::AllowDouble |
            fastgltf::Options::LoadExternalBuffers |
            fastgltf::Options::LoadExternalImages |
            fastgltf::Options::GenerateMeshIndices;


        auto gltfFile = fastgltf::MappedGltfFile::FromPath(path);
        if (!(gltfFile)) {
            std::cerr << "Failed to open glTF file: " << fastgltf::getErrorMessage(gltfFile.error()) << '\n';
        }

        auto asset = parser.loadGltf(gltfFile.get(), path.parent_path(), gltfOptions);
        if (asset.error() != fastgltf::Error::None) {
            std::cerr << "Failed to load glTF: " << fastgltf::getErrorMessage(asset.error()) << '\n';
        }

        // process Nodes
        for ( auto& node : asset->nodes)
        ProcessNode( node , asset.get() ,model) ;
        return model;

    }
}

    void ModelLoader::ProcessNode( fastgltf::Node &node, fastgltf::Asset &asset , Model& model )  {
      // process current node

    if (!node.meshIndex .has_value())
        return ;
        model.meshes.push_back(ProcessMesh( asset.meshes[*node.meshIndex], asset ));
     // recursively process children
      for (const auto& childIndex : node.children) {
          auto& childNode = asset.nodes[childIndex];
          ProcessNode(childNode, asset, model);
      }
    };

    Mesh ModelLoader::ProcessMesh(fastgltf::Mesh &mesh , const fastgltf::Asset &asset  ) {
        std::vector<Vertex> vertices ;
        std::vector<unsigned int> indices ;

        fastgltf::Primitive& primitive = mesh.primitives[0];
        if (primitive.indicesAccessor.has_value()) {
            auto& accessor = asset.accessors[primitive.indicesAccessor.value()];
            indices.resize(accessor.count);

            std::size_t idx = 0;
            fastgltf::iterateAccessor<std::uint32_t>(asset, accessor, [&](unsigned int  index) {
                indices[idx++] = index;
            });

        }

        // process vertices
        const auto* positionIt = primitive.findAttribute("POSITION");
        auto& posAccessor = asset.accessors[positionIt->accessorIndex];

        vertices.resize(posAccessor.count);

        // positions
        fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, posAccessor,
     [&](fastgltf::math::fvec3 p, size_t i) {
         vertices[i].position = glm::vec3(p.x(), p.y(), p.z());
     }
        );

        // normals
        const auto* normalIt = primitive.findAttribute("NORMAL") ;
        auto& normalAccessor = asset.accessors[normalIt->accessorIndex];


        if ( bool validNormal =  normalAccessor.type == fastgltf::AccessorType::Vec3 ) {
            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, normalAccessor,
                [&](fastgltf::math::fvec3 n, size_t i) {
                    vertices[i].normal = glm::vec3(n.x(), n.y(), n.z());
                }
            );
        }
        else {
            // if no normals are present, set default normals
            for (auto& vertex : vertices) {
                vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
            }
        }

        // tangents

        const auto* uvIt = primitive.findAttribute("TEXCOORD_0");
        auto& uvAccessor = asset.accessors[uvIt->accessorIndex];

        if ( bool validUV = uvAccessor.type == fastgltf::AccessorType::Vec2 ) {
            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(asset, uvAccessor,
                [&](fastgltf::math::fvec2 uv, size_t i) {
                    vertices[i].uv = glm::vec2(uv.x(), uv.y());
                }
            );
        }
        else {
            // if no uvs are present, set default uvs
            for (auto& vertex : vertices) {
                vertex.uv = glm::vec2(0.0f, 0.0f);
            }
        }

        const auto* tangentIt = primitive.findAttribute("TANGENT");
        auto& tangentAccessor = asset.accessors[tangentIt->accessorIndex];

        if ( bool validTangent = tangentAccessor.type == fastgltf::AccessorType::Vec4  ) {
            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(asset, tangentAccessor,
                [&](fastgltf::math::fvec4 t, const size_t i) {
                    vertices[i].tangent = glm::vec4(t.x(), t.y(), t.z(),t.w());
                }
            );
        }
        else {
            // if no tangents are present, set default tangents
            for (auto& vertex : vertices) {
                vertex.tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
        }

        return Mesh{vertices, indices , {} ,{}} ;
    }