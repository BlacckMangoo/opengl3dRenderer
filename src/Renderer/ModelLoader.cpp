#include <Renderer/ModelLoader.h>
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <iostream>


Mesh ModelLoader::LoadGLTF( const std::filesystem::path &path) {
    // Parse the glTF file and get the constructed asset
    {
        Mesh model;


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

    void ModelLoader::ProcessNode( fastgltf::Node &node, fastgltf::Asset &asset , Mesh& model )  {
      // process current node

    if (!node.meshIndex .has_value())        return ;
    model.mesh.push_back(ProcessPrimtives( asset.meshes[*node.meshIndex], asset ));
     // recursively process children
      for (const auto& childIndex : node.children) {
          auto& childNode = asset.nodes[childIndex];
          ProcessNode(childNode, asset, model);
      }
    };


    Primitive ModelLoader::ProcessPrimtives(fastgltf::Mesh &mesh , const fastgltf::Asset &asset  ) {
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
        const auto* normalIt = primitive.findAttribute("NORMAL");
        bool hasNormals = normalIt && normalIt->accessorIndex < asset.accessors.size();
        if (hasNormals) {
            auto& normalAccessor = asset.accessors[normalIt->accessorIndex];
            if (normalAccessor.type == fastgltf::AccessorType::Vec3) {
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, normalAccessor,
                    [&](fastgltf::math::fvec3 n, const size_t i) {
                        vertices[i].normal = glm::vec3(n.x(), n.y(), n.z());
                    }
                );
            } else {
                hasNormals = false; // Fallback to generating normals
            }
        }
        if (!hasNormals) {
            // Generate normals from positions and indices
            // Initialize normals to zero
            for (auto& vertex : vertices) {
                vertex.normal = glm::vec3(0.0f);
            }
            // For each triangle, compute face normal and accumulate
            for (size_t i = 0; i + 2 < indices.size(); i += 3) {
                unsigned int ia = indices[i];
                unsigned int ib = indices[i + 1];
                unsigned int ic = indices[i + 2];
                if (ia >= vertices.size() || ib >= vertices.size() || ic >= vertices.size()) continue;
                glm::vec3& a = vertices[ia].position;
                glm::vec3& b = vertices[ib].position;
                glm::vec3& c = vertices[ic].position;
                const glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
                vertices[ia].normal += normal;
                vertices[ib].normal += normal;
                vertices[ic].normal += normal;
            }
            // Normalize all normals
            for (auto& vertex : vertices) {
                vertex.normal = glm::normalize(vertex.normal);
            }
            std::cout << "Generated normals for mesh: " << mesh.name << std::endl;
        }

        // tangents
        if (const auto* uvIt = primitive.findAttribute("TEXCOORD_0"); uvIt && uvIt->accessorIndex < asset.accessors.size()) {
            auto& uvAccessor = asset.accessors[uvIt->accessorIndex];
            if ( bool validUV = uvAccessor.type == fastgltf::AccessorType::Vec2 ) {
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(asset, uvAccessor,
                    [&](fastgltf::math::fvec2 uv, size_t i) {
                        vertices[i].uv = glm::vec2(uv.x(), uv.y());
                    }
                );
            }
            else {
                for (auto& vertex : vertices) {
                    vertex.uv = glm::vec2(0.0f, 0.0f);
                }
            }
        } else {
            for (auto& vertex : vertices) {
                vertex.uv = glm::vec2(0.0f, 0.0f);
            }
        }

        const auto* tangentIt = primitive.findAttribute("TANGENT");
        if (tangentIt && tangentIt->accessorIndex < asset.accessors.size()) {
            auto& tangentAccessor = asset.accessors[tangentIt->accessorIndex];
            if ( bool validTangent = tangentAccessor.type == fastgltf::AccessorType::Vec4  ) {
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(asset, tangentAccessor,
                    [&](fastgltf::math::fvec4 t, const size_t i) {
                        vertices[i].tangent = glm::vec4(t.x(), t.y(), t.z(),t.w());
                    }
                );
            }
            else {
                for (auto& vertex : vertices) {
                    vertex.tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        } else {
            for (auto& vertex : vertices) {
                vertex.tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
        }

        return Primitive{vertices, indices , {} ,{}} ;
    }