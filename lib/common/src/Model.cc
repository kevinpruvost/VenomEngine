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

#include <mikktspace.h>
#include <assimp/GltfMaterial.h>

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

void Model::_GUI()
{
    vc::String newPath;
    if (vc::GUI::EditableModel(this, newPath)) {
        if (vc::Error err = ImportModel(newPath.c_str()); err != vc::Error::Success) {
            vc::Log::Error("Failed to load model from file: %s", newPath.c_str());
        } else {

        }
    }
}

vc::String Model::_GetComponentTitle()
{
    return "Model";
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
        type = MaterialComponentValueType::FLOAT1D;
    } else if (strncmp(name.c_str(), "$clr.", 4) == 0) {
        type = MaterialComponentValueType::FLOAT3D;
        if (dataLength == sizeof(float) * 4) type = MaterialComponentValueType::FLOAT4D;
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

    _ResetResource();
    // Get Parent folder for relative paths when we will load textures
    auto parentFolder = std::filesystem::path(realPath).parent_path();

    // Create Logger
    if (Assimp::DefaultLogger::isNullLogger())
        Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);
    Assimp::Importer importer;
    //importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 45.0f); // Example: 45 degrees
    // importer.SetPropertyInteger(AI_CONFIG_PP_CT_MAX_SMOOTHING_ANGLE, 30);
    // Print cwd
    const aiScene* scene = importer.ReadFile(realPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices);
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
                    case MaterialComponentValueType::FLOAT1D: {
                        float value;
                        memcpy(&value, property->mData, sizeof(float));
                        material.SetComponent(matCompType, value);
                        break;
                    }
                    case MaterialComponentValueType::FLOAT3D: {
                        aiColor3D value;
                        memcpy(&value, property->mData, sizeof(aiColor3D));
                        material.SetComponent(matCompType, vcm::Vec3(value.r, value.g, value.b));
                        break;
                    }
                    case MaterialComponentValueType::FLOAT4D: {
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
                        // If .gltf, then we have to load another way
                        if (std::filesystem::path(path).extension() == ".glb") {
                            // Load gltf texture
                            unsigned int textureIndex = std::atoi(value.C_Str() + 1);
                            aiTexture* aiTexture = scene->mTextures[textureIndex];

                            Texture texture(path, textureIndex, reinterpret_cast<char*>(aiTexture->pcData), aiTexture->mWidth, aiTexture->mHeight);
                            material.SetComponent(matCompType, texture);
                        } else {
                            Texture texture(texturePath.c_str());
                            material.SetComponent(matCompType, texture);
                        }
                        break;
                    }
                    default:
                        break;
                }

                switch (matCompType)
                {
                    case MaterialComponentType::DIFFUSE:
                        material.SetComponent(MaterialComponentType::BASE_COLOR, material.GetComponent(MaterialComponentType::DIFFUSE));
                        break;
                    case MaterialComponentType::ROUGHNESS: {
                        if (valueType == MaterialComponentValueType::TEXTURE) {
                            // Find corresponding channel
                            aiString test;
                            if (aimaterial->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &test) == AI_SUCCESS) {
                                // Convention says that the roughness is in the G channel
                                material.SetComponentChannels(MaterialComponentType::ROUGHNESS, MaterialComponentValueChannels::G);
                            }
                            float roughnessFactor = 1.0f;
                            if (aimaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughnessFactor) == AI_SUCCESS) {
                                material.SetComponent(MaterialComponentType::ROUGHNESS, roughnessFactor);
                            }
                        }
                    }
                    case MaterialComponentType::METALLIC: {
                        if (valueType == MaterialComponentValueType::TEXTURE) {
                            // Find corresponding channel
                            aiString test;
                            if (aimaterial->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &test) == AI_SUCCESS) {
                                // Convention says that the metallic is in the B channel
                                material.SetComponentChannels(MaterialComponentType::METALLIC, MaterialComponentValueChannels::B);
                            }
                            float metallicFactor = 1.0f;
                            if (aimaterial->Get(AI_MATKEY_METALLIC_FACTOR, metallicFactor) == AI_SUCCESS) {
                                material.SetComponent(MaterialComponentType::METALLIC, metallicFactor);
                            }
                        }
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
    // Find the bounding box for normalization
    glm::vec3 minVertex(FLT_MAX);
    glm::vec3 maxVertex(-FLT_MAX);
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        vc::Mesh & mesh = _resource->As<ModelResource>()->meshes.emplace_back();

        const aiMesh* aimesh = scene->mMeshes[i];

        // Assign material
        mesh.SetMaterial(_resource->As<ModelResource>()->materials[aimesh->mMaterialIndex]);

        // Vertices & normals
        mesh._impl->As<MeshImpl>()->_positions.reserve(aimesh->mNumVertices);
        mesh._impl->As<MeshImpl>()->_normals.reserve(aimesh->mNumVertices);

        for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
            const auto& vertex = aimesh->mVertices[x];
            minVertex = glm::min(minVertex, glm::vec3(vertex.x, vertex.y, vertex.z));
            maxVertex = glm::max(maxVertex, glm::vec3(vertex.x, vertex.y, vertex.z));
        }

        // Normalize vertices & normals
        for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
            vcm::VertexPos position = glm::vec3(aimesh->mVertices[x].x, aimesh->mVertices[x].y, aimesh->mVertices[x].z);
            mesh._impl->As<MeshImpl>()->_positions.emplace_back(position);

            if (aimesh->HasNormals()) {
                vcm::VertexNormal normal = glm::vec3(aimesh->mNormals[x].x, aimesh->mNormals[x].y, aimesh->mNormals[x].z);
                mesh._impl->As<MeshImpl>()->_normals.emplace_back(normal);
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

        // if (aimesh->HasTangentsAndBitangents()) {
        //     mesh._impl->As<MeshImpl>()->_tangents.reserve(aimesh->mNumVertices);
        //     mesh._impl->As<MeshImpl>()->_bitangents.reserve(aimesh->mNumVertices);
        //     for (uint32_t x = 0; x < aimesh->mNumVertices; ++x) {
        //         mesh._impl->As<MeshImpl>()->_tangents.emplace_back(aimesh->mTangents[x].x, aimesh->mTangents[x].y, aimesh->mTangents[x].z);
        //         mesh._impl->As<MeshImpl>()->_bitangents.emplace_back(aimesh->mBitangents[x].x, aimesh->mBitangents[x].y, aimesh->mBitangents[x].z);
        //     }
        // } else
        {
            // MikkTSpace
            // Tangents & Bitangents
            if (aimesh->HasTextureCoords(0)) {
                mesh._impl->As<MeshImpl>()->_tangents.resize(aimesh->mNumVertices);
                mesh._impl->As<MeshImpl>()->_bitangents.resize(aimesh->mNumVertices);

                // MikkTSpace
                struct MikkTSpaceData {
                    const aiMesh * aimesh;
                    std::vector<glm::vec3> * tangents;
                    std::vector<glm::vec3> * bitangents;
                } mikktspaceData = { aimesh, &mesh._impl->As<MeshImpl>()->_tangents, &mesh._impl->As<MeshImpl>()->_bitangents };

                SMikkTSpaceInterface mikktspace;
                mikktspace.m_getNumFaces = [](const SMikkTSpaceContext * pContext) -> int {
                    return reinterpret_cast<MikkTSpaceData*>(pContext->m_pUserData)->aimesh->mNumFaces;
                };
                mikktspace.m_getNumVerticesOfFace = [](const SMikkTSpaceContext * pContext, const int iFace) -> int {
                    return reinterpret_cast<MikkTSpaceData*>(pContext->m_pUserData)->aimesh->mFaces[iFace].mNumIndices;
                };
                mikktspace.m_getPosition = [](const SMikkTSpaceContext * pContext, float fvPosOut[], const int iFace, const int iVert) {
                    MikkTSpaceData* data = reinterpret_cast<MikkTSpaceData*>(pContext->m_pUserData);
                    const aiVector3D& pos = data->aimesh->mVertices[data->aimesh->mFaces[iFace].mIndices[iVert]];
                    fvPosOut[0] = pos.x;
                    fvPosOut[1] = pos.y;
                    fvPosOut[2] = pos.z;
                };
                mikktspace.m_getNormal = [](const SMikkTSpaceContext * pContext, float fvNormOut[], const int iFace, const int iVert) {
                    MikkTSpaceData* data = reinterpret_cast<MikkTSpaceData*>(pContext->m_pUserData);
                    const aiVector3D& normal = data->aimesh->mNormals[data->aimesh->mFaces[iFace].mIndices[iVert]];
                    fvNormOut[0] = normal.x;
                    fvNormOut[1] = normal.y;
                    fvNormOut[2] = normal.z;
                };
                mikktspace.m_getTexCoord = [](const SMikkTSpaceContext * pContext, float fvTexcOut[], const int iFace, const int iVert) {
                    MikkTSpaceData* data = reinterpret_cast<MikkTSpaceData*>(pContext->m_pUserData);
                    const aiVector3D& uv = data->aimesh->mTextureCoords[0][data->aimesh->mFaces[iFace].mIndices[iVert]];
                    fvTexcOut[0] = uv.x;
                    fvTexcOut[1] = uv.y;
                };
                mikktspace.m_setTSpaceBasic = [](const SMikkTSpaceContext * pContext, const float fvTangent[], const float fSign, const int iFace, const int iVert) {
                    MikkTSpaceData * data = reinterpret_cast<MikkTSpaceData *>(pContext->m_pUserData);
                    int index = data->aimesh->mFaces[iFace].mIndices[iVert];
                    vcm::Vec3 normal(data->aimesh->mNormals[index].x, data->aimesh->mNormals[index].y, data->aimesh->mNormals[index].z);
                    vcm::Vec3 tangent(fvTangent[0], fvTangent[1], fvTangent[2]);
                    data->tangents->at(index) = tangent;
                    data->bitangents->at(index) = vcm::CrossProduct(normal, tangent) * fSign;
                };
                mikktspace.m_setTSpace = nullptr;

                SMikkTSpaceContext mikktspaceContext;
                mikktspaceContext.m_pInterface = &mikktspace;
                mikktspaceContext.m_pUserData = &mikktspaceData;

                genTangSpaceDefault(&mikktspaceContext);
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
    }

    // Center and scale factor calculation
    glm::vec3 center = (minVertex + maxVertex) * 0.5f;
    float maxExtent = glm::length(maxVertex - center) * 0.5f;

    // Scaling
    for (auto& mesh : _resource->As<ModelResource>()->meshes) {
        for (auto& position : mesh._impl->As<MeshImpl>()->_positions) {
            position = (position - center) / maxExtent;
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

const vc::Vector<vc::Material>& ModelImpl::GetMaterials() const
{
    return _resource->As<ModelResource>()->materials;
}

vc::Vector<vc::Material>& ModelImpl::GetMaterials()
{
    return _resource->As<ModelResource>()->materials;
}
}
}
