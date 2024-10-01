///
/// Project: VenomEngine
/// @file Mesh.cc
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Model.h>

#include <venom/common/VenomEngine.h>
#include <venom/common/Resources.h>
#include <venom/common/Log.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

namespace venom
{
namespace common
{
Model::Model()
    : GraphicsPluginObject()
{
}

Model* Model::Create()
{
    return GraphicsPlugin::Get()->CreateModel();
}

Model::~Model()
{
}

vc::Error Model::ImportModel(const std::string & path)
{
    auto resPath = vc::Resources::GetModelsResourcePath(path);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(resPath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    if (!scene) {
        vc::Log::Error("Failed to load model: %s", path.c_str());
        return vc::Error::Failure;
    }

    // Load every mesh
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        auto mesh = vc::Mesh::Create();
        __meshes.push_back(mesh);

        const aiMesh* aimesh = scene->mMeshes[i];

        // Vertices & normals
        mesh->__positions.reserve(aimesh->mNumVertices);
        mesh->__normals.reserve(aimesh->mNumVertices);
        for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
            mesh->__positions.emplace_back(aimesh->mVertices[x].x, aimesh->mVertices[x].y, aimesh->mVertices[x].z);
            mesh->__normals.emplace_back(aimesh->mNormals[x].x, aimesh->mNormals[x].y, aimesh->mNormals[x].z);
        }

        // Color sets
        for (int c = 0; c < AI_MAX_NUMBER_OF_COLOR_SETS; ++c) {
            if (!aimesh->HasVertexColors(c)) break;

            mesh->__colors[c].reserve(aimesh->mNumVertices);
            for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
                mesh->__colors[c].emplace_back(aimesh->mColors[c][x].r, aimesh->mColors[c][x].g, aimesh->mColors[c][x].b, aimesh->mColors[c][x].a);
            }
        }

        // UV Texture Coords
        for (int c = 0; c < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++c) {
            if (!aimesh->HasTextureCoords(c)) break;

            mesh->__uvs[c].reserve(aimesh->mNumVertices);
            for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
                mesh->__uvs[c].emplace_back(aimesh->mTextureCoords[c][x].x, aimesh->mTextureCoords[c][x].y);
            }
        }

        // Tangents & Bitangents
        if (aimesh->HasTangentsAndBitangents()) {
            mesh->__tangents.reserve(aimesh->mNumVertices);
            mesh->__bitangents.reserve(aimesh->mNumVertices);
            for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
                mesh->__tangents.emplace_back(aimesh->mTangents[x].x, aimesh->mTangents[x].y, aimesh->mTangents[x].z);
                mesh->__bitangents.emplace_back(aimesh->mBitangents[x].x, aimesh->mBitangents[x].y, aimesh->mBitangents[x].z);
            }
        }

        // Faces
        if (aimesh->HasFaces()) {
            mesh->__indices.reserve(aimesh->mNumFaces * 3);
            for (uint32_t x = 0; x < aimesh->mNumFaces; ++x) {
                mesh->__indices.push_back(aimesh->mFaces[x].mIndices[0]);
                mesh->__indices.push_back(aimesh->mFaces[x].mIndices[1]);
                mesh->__indices.push_back(aimesh->mFaces[x].mIndices[2]);
            }
        }

        // Load mesh into Graphics API
        if (auto err = mesh->__LoadMeshFromCurrentData(); err != vc::Error::Success) {
            vc::Log::Error("Failed to load mesh from current data");
            return err;
        }
    }
    return vc::Error::Success;
}

const std::vector<vc::Mesh*>& Model::GetMeshes() const
{
    return __meshes;
}
}
}
