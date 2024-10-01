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

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        auto mesh = vc::Mesh::Create();
    //    mesh->ImportMesh(scene->mMeshes[i]);
        __meshes.push_back(mesh);
    }

    aiReleaseImport(scene);
    return vc::Error::Success;
}
}
}
