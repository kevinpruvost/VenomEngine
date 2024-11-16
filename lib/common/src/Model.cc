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

#include <iostream>
#include <assimp/DefaultLogger.hpp>
#include <filesystem>

namespace venom
{
namespace common
{
ModelImpl::ModelImpl()
    : GraphicsPluginObject()
    , PluginObjectImpl()
{
}

Model::Model()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateModel())
{
}

Model::Model(const char * path)
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateModel())
{
    if (Error err = ImportModel(path); err != Error::Success) {
        _impl->As<ModelImpl>()->Destroy();
    }
}

Model::~Model()
{
}

static MaterialComponentType GetMaterialComponentTypeFromAiTextureType(const aiTextureType type)
{
    switch (type)
    {
        case aiTextureType_DIFFUSE: return MaterialComponentType::DIFFUSE;
        case aiTextureType_SPECULAR: return MaterialComponentType::SPECULAR;
        case aiTextureType_AMBIENT: return MaterialComponentType::AMBIENT;
        case aiTextureType_EMISSIVE: return MaterialComponentType::EMISSIVE;
        case aiTextureType_HEIGHT: return MaterialComponentType::HEIGHT;
        case aiTextureType_NORMALS: return MaterialComponentType::NORMAL;
        case aiTextureType_SHININESS: return MaterialComponentType::SHININESS;
        case aiTextureType_OPACITY: return MaterialComponentType::OPACITY;
        case aiTextureType_REFLECTION: return MaterialComponentType::REFLECTION;
        case aiTextureType_BASE_COLOR: return MaterialComponentType::BASE_COLOR;
        case aiTextureType_METALNESS: return MaterialComponentType::METALLIC;
        case aiTextureType_DIFFUSE_ROUGHNESS: return MaterialComponentType::ROUGHNESS;
        case aiTextureType_AMBIENT_OCCLUSION: return MaterialComponentType::AMBIENT_OCCLUSION;
        case aiTextureType_EMISSION_COLOR: return MaterialComponentType::EMISSION_COLOR;
        case aiTextureType_TRANSMISSION: return MaterialComponentType::TRANSMISSION;
        case aiTextureType_SHEEN: return MaterialComponentType::SHEEN;
        case aiTextureType_CLEARCOAT: return MaterialComponentType::CLEARCOAT;
        default: return MaterialComponentType::MAX_COMPONENT;
    }
}

static MaterialComponentType GetMaterialComponentTypeFromProperty(const std::string & name, const int semantic, const int index, const int dataLength, MaterialComponentValueType & type)
{
    // If name starts with "$mat." it's a value, "$clr." is a color, "$tex.file" is a texture
    if (strncmp(name.c_str(), "$mat.", 4) == 0) {
        type = MaterialComponentValueType::VALUE;
    } else if (strncmp(name.c_str(), "$clr.", 4) == 0) {
        type = MaterialComponentValueType::COLOR3D;
        if (dataLength == sizeof(float) * 4) type = MaterialComponentValueType::COLOR4D;
    } else if (strncmp(name.c_str(), "$tex.file", 9) == 0) {
        type = MaterialComponentValueType::TEXTURE;
        return GetMaterialComponentTypeFromAiTextureType(static_cast<aiTextureType>(semantic));
    } else {
        type = MaterialComponentValueType::NONE;
    }

    if (name == "$clr.diffuse") return MaterialComponentType::DIFFUSE;
    if (name == "$clr.ambient") return MaterialComponentType::AMBIENT;
    if (name == "$clr.specular") return MaterialComponentType::SPECULAR;
    if (name == "$clr.emissive") return MaterialComponentType::EMISSIVE;
    if (name == "$mat.shininess") return MaterialComponentType::SHININESS;
    if (name == "$mat.opacity") return MaterialComponentType::OPACITY;
    if (name == "$mat.anisotropyFactor") return MaterialComponentType::ANISOTROPY;
    if (name == "$clr.transparent") return MaterialComponentType::TRANSPARENT;
    if (name == "$clr.reflective") return MaterialComponentType::REFLECTION;
    if (name == "$mat.refracti") return MaterialComponentType::REFRACTION;
    if (name == "$mat.reflectivity") return MaterialComponentType::REFLECTIVITY;


    return MaterialComponentType::MAX_COMPONENT;
}

// static vc::Error ProcessNode(aiNode * node, const aiScene * scene, aiMatrix4x4 transformation, std::vector<vc::Mesh> & meshes, std::vector<vc::Material> & materials)
// {
//     int mNumC = node->mNumMeshes;
//     for (int i = 0; i < node->mNumChildren; ++i) {
//         ProcessNode(node->mChildren[i], scene, transformation * node->mChildren[i]->mTransformation, meshes, materials);
//     }
//     return vc::Error::Success;
// }

vc::Error ModelImpl::ImportModel(const char * path)
{
    auto realPath = Resources::GetModelsResourcePath(path);

    {
        // Load from cache if already loaded
        std::shared_ptr<GraphicsCachedResource> cachedModel = GraphicsPluginObject::GetCachedObject(realPath);
        if (cachedModel) {
            _LoadFromCache(cachedModel);
            return vc::Error::Success;
        }
    }

    // Get Parent folder for relative paths when we will load textures
    auto parentFolder = std::filesystem::path(realPath).parent_path();

    // Create Logger
    if (Assimp::DefaultLogger::isNullLogger())
        Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);
    Assimp::Importer importer;
    // Print cwd
    const aiScene* scene = importer.ReadFile(realPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices);
    if (!scene) {
        vc::Log::Error("Failed to load model: %s", realPath.c_str());
        return vc::Error::Failure;
    }

    // Load every material
    if (scene->HasMaterials()) {
        _resource->As<ModelResource>()->materials.reserve(scene->mNumMaterials);
        for (uint32_t i = 0; i < scene->mNumMaterials; ++i) {
            vc::Material & material = _resource->As<ModelResource>()->materials.emplace_back();

            const aiMaterial* aimaterial = scene->mMaterials[i];

            // Iterate over all properties of the material
            for (unsigned int p = 0; p < aimaterial->mNumProperties; ++p) {
                aiMaterialProperty* property = aimaterial->mProperties[p];

                // Property Key (name) and Type
                auto propName = property->mKey.C_Str();
                auto propType = property->mType;
                auto propIndex = property->mIndex;
                auto propSemantic = property->mSemantic;

                // If propName is "?mat.name", it's the material name
                if (strncmp(propName, "?mat.name", 9) == 0) {
                    aiString value;
                    memcpy(&value, property->mData, property->mDataLength);
                    material.SetName(value.C_Str());
                    continue;
                }

                MaterialComponentValueType valueType;
                MaterialComponentType matCompType = GetMaterialComponentTypeFromProperty(property->mKey.C_Str(), property->mSemantic, property->mIndex, property->mDataLength, valueType);

                if (matCompType == MaterialComponentType::MAX_COMPONENT) {
                    vc::Log::Error("Unknown material component type: %s", property->mKey.C_Str());
                    continue;
                }

                switch (valueType) {
                    case MaterialComponentValueType::VALUE: {
                        float value;
                        memcpy(&value, property->mData, sizeof(float));
                        material.SetComponent(matCompType, value);
                        break;
                    }
                    case MaterialComponentValueType::COLOR3D: {
                        aiColor3D value;
                        memcpy(&value, property->mData, sizeof(aiColor3D));
                        material.SetComponent(matCompType, vcm::Vec3(value.r, value.g, value.b));
                        break;
                    }
                    case MaterialComponentValueType::COLOR4D: {
                        aiColor4D value;
                        memcpy(&value, property->mData, sizeof(aiColor4D));
                        material.SetComponent(matCompType, vcm::Vec4(value.r, value.g, value.b, value.a));
                        break;
                    }
                    case MaterialComponentValueType::TEXTURE: {
                        aiString value;
                        memcpy(&value, property->mData, property->mDataLength);
                        // Tries to load from cache or path
                        std::string texturePath = parentFolder / value.C_Str();
                        Texture texture(texturePath.c_str());
                        material.SetComponent(matCompType, texture);
                        break;
                    }
                    default:
                        break;
                }

#ifdef VENOM_DEBUG
                Log::LogToFile("Property Name: %s", property->mKey.C_Str());
                Log::LogToFile("Property Semantic: %d", property->mSemantic);
                Log::LogToFile("Property Index: %d", property->mIndex);
                Log::LogToFile("Property Data Length: %d", property->mDataLength);
                Log::LogToFile("Property Type: %d", property->mType);

                // Check property type
                switch (property->mType) {
                case aiPTI_Float:
                        Log::LogToFile("Float\n");
                        break;
                case aiPTI_Integer:
                        Log::LogToFile("Integer\n");
                        break;
                case aiPTI_String:
                        Log::LogToFile("String\n");
                        break;
                case aiPTI_Buffer:
                        Log::LogToFile("Buffer\n");
                        break;
                default:
                        Log::LogToFile("Unknown\n");
                }

                // Handle different property types
                if (property->mType == aiPTI_Float && property->mDataLength == sizeof(float)) {
                    float value;
                    memcpy(&value, property->mData, sizeof(float));
                    Log::LogToFile("Float Value: %f\n", value);
                } else if (property->mType == aiPTI_Integer && property->mDataLength == sizeof(int)) {
                    int value;
                    memcpy(&value, property->mData, sizeof(int));
                    Log::LogToFile("Integer Value: %d\n", value);
                } else if (property->mType == aiPTI_String) {
                    aiString value;
                    memcpy(&value, property->mData, property->mDataLength);
                    Log::LogToFile("String Value: %s\n", value.C_Str());
                }

                Log::LogToFile("--------------------------------------------\n");
#endif
            }
        }
    }

    _resource->As<ModelResource>()->meshes.reserve(scene->mNumMeshes);
    // vc::Error err = ProcessNode(scene->mRootNode, scene, scene->mRootNode->mTransformation, _resource->As<ModelResource>()->meshes, _resource->As<ModelResource>()->materials);
    // if (err != vc::Error::Success) {
    //     vc::Log::Error("Failed to process node");
    //     return err;
    // }

    // Load every mesh
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        vc::Mesh & mesh = _resource->As<ModelResource>()->meshes.emplace_back();

        const aiMesh* aimesh = scene->mMeshes[i];

        // Assign material
        mesh.SetMaterial(_resource->As<ModelResource>()->materials[aimesh->mMaterialIndex]);

        // Vertices & normals
        mesh._impl->As<MeshImpl>()->_positions.reserve(aimesh->mNumVertices);
        mesh._impl->As<MeshImpl>()->_normals.reserve(aimesh->mNumVertices);

        // Find the bounding box for normalization
        glm::vec3 minVertex(FLT_MAX);
        glm::vec3 maxVertex(-FLT_MAX);

        for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
            const auto& vertex = aimesh->mVertices[x];
            minVertex = glm::min(minVertex, glm::vec3(vertex.x, vertex.y, vertex.z));
            maxVertex = glm::max(maxVertex, glm::vec3(vertex.x, vertex.y, vertex.z));
        }

        // Center and scale factor calculation
        glm::vec3 center = (minVertex + maxVertex) * 0.5f;
        float maxExtent = glm::length(maxVertex - center) * 0.5f;

        // Normalize vertices & normals
        for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
            glm::vec3 position = glm::vec3(aimesh->mVertices[x].x, aimesh->mVertices[x].y, aimesh->mVertices[x].z);
            position = (position - center) / maxExtent;
            mesh._impl->As<MeshImpl>()->_positions.emplace_back(position);

            if (aimesh->HasNormals()) {
                glm::vec3 normal = glm::vec3(aimesh->mNormals[x].x, aimesh->mNormals[x].y, aimesh->mNormals[x].z);
                mesh._impl->As<MeshImpl>()->_normals.emplace_back(glm::normalize(normal));
            }
        }

        // Color sets
        for (int c = 0; c < AI_MAX_NUMBER_OF_COLOR_SETS; ++c) {
            if (!aimesh->HasVertexColors(c)) break;

            mesh._impl->As<MeshImpl>()->_colors[c].reserve(aimesh->mNumVertices);
            for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
                mesh._impl->As<MeshImpl>()->_colors[c].emplace_back(aimesh->mColors[c][x].r, aimesh->mColors[c][x].g, aimesh->mColors[c][x].b, aimesh->mColors[c][x].a);
            }
        }

        // UV Texture Coords
        for (int c = 0; c < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++c) {
            if (!aimesh->HasTextureCoords(c)) break;

            mesh._impl->As<MeshImpl>()->_uvs[c].reserve(aimesh->mNumVertices);
            for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
                mesh._impl->As<MeshImpl>()->_uvs[c].emplace_back(aimesh->mTextureCoords[c][x].x, aimesh->mTextureCoords[c][x].y);
            }
        }

        // Tangents & Bitangents
        if (aimesh->HasTangentsAndBitangents()) {
            mesh._impl->As<MeshImpl>()->_tangents.reserve(aimesh->mNumVertices);
            mesh._impl->As<MeshImpl>()->_bitangents.reserve(aimesh->mNumVertices);
            for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
                mesh._impl->As<MeshImpl>()->_tangents.emplace_back(aimesh->mTangents[x].x, aimesh->mTangents[x].y, aimesh->mTangents[x].z);
                mesh._impl->As<MeshImpl>()->_bitangents.emplace_back(aimesh->mBitangents[x].x, aimesh->mBitangents[x].y, aimesh->mBitangents[x].z);
            }
        }

        // Faces
        if (aimesh->HasFaces()) {
            mesh._impl->As<MeshImpl>()->_indices.reserve(aimesh->mNumFaces * 3);
            for (uint32_t x = 0; x < aimesh->mNumFaces; ++x) {
                mesh._impl->As<MeshImpl>()->_indices.push_back(aimesh->mFaces[x].mIndices[0]);
                mesh._impl->As<MeshImpl>()->_indices.push_back(aimesh->mFaces[x].mIndices[1]);
                mesh._impl->As<MeshImpl>()->_indices.push_back(aimesh->mFaces[x].mIndices[2]);
            }
        }

        // Load mesh into Graphics API
        if (auto err = mesh._impl->As<MeshImpl>()->__LoadMeshFromCurrentData(); err != vc::Error::Success) {
            vc::Log::Error("Failed to load mesh from current data");
            return err;
        }
    }
    _SetInCache(realPath, _resource);
    return vc::Error::Success;
}

const std::vector<vc::Mesh> & ModelImpl::GetMeshes() const
{
    return _resource->As<ModelResource>()->meshes;
}
}
}
