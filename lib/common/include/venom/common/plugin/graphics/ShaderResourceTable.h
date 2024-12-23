///
/// Project: VenomEngineWorkspace
/// @file ShaderResourceTable.h
/// @date Oct, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/plugin/graphics/Texture.h>
#include <venom/common/math/Matrix.h>
#include <venom/common/VenomSettings.h>


// @brief Means that all model matrices will be located in a packed buffer
#define VENOM_EXTERNAL_PACKED_MODEL_MATRIX

// @brief Means that all textures will be bindless, MoltenVk only allows for small amounts of bindless textures
#ifndef __APPLE__
#define VENOM_BINDLESS_TEXTURES
#endif

namespace venom
{
namespace common
{
class Texture;


class ShaderResourceTable : public GraphicsPluginObject
{
public:
    ShaderResourceTable();
    ~ShaderResourceTable() override;

    enum SetsIndex
    {
        SetsIndex_ModelMatrices = 0,
        SetsIndex_Camera = 1,
        SetsIndex_Textures = 2,
        SetsIndex_Sampler = 3,
        SetsIndex_Material = 4,
        SetsIndex_Scene = 5,
        SetsIndex_Panorama = 6,
        SetsIndex_Light = 7
    };

// Descriptor Sets Indices
#define DSETS_INDEX_MODEL_MATRICES venom::common::ShaderResourceTable::SetsIndex::SetsIndex_ModelMatrices
#define DSETS_INDEX_CAMERA venom::common::ShaderResourceTable::SetsIndex::SetsIndex_Camera
#define DSETS_INDEX_TEXTURES venom::common::ShaderResourceTable::SetsIndex::SetsIndex_Textures
#define DSETS_INDEX_SAMPLER venom::common::ShaderResourceTable::SetsIndex::SetsIndex_Sampler
#define DSETS_INDEX_MATERIAL venom::common::ShaderResourceTable::SetsIndex::SetsIndex_Material
#define DSETS_INDEX_SCENE venom::common::ShaderResourceTable::SetsIndex::SetsIndex_Scene
#define DSETS_INDEX_PANORAMA venom::common::ShaderResourceTable::SetsIndex::SetsIndex_Panorama
#define DSETS_INDEX_LIGHT venom::common::ShaderResourceTable::SetsIndex::SetsIndex_Light
    
    class Descriptor
    {
    public:
        Descriptor();
        ~Descriptor();
    };

    static void UpdateDescriptor(const SetsIndex index, const int binding, const void * data, const size_t size, const size_t offset = 0);
    static void UpdateDescriptor(const SetsIndex index, const int binding, vc::Texture * texture);

#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    static vcm::Mat4 * GetAllModelMatrixBuffer();
    static vcm::Mat4 * GetModelMatrixBuffer();
    static inline size_t GetAllModelMatrixBytesSize() { return VENOM_MAX_ENTITIES * sizeof(vcm::Mat4); }
    static void ReleaseModelMatrixBuffer(const vcm::Mat4 * mat);
    static int GetModelMatrixBufferId(const vcm::Mat4 * mat);
#endif

    static int BindTexture();
    static void UnbindTexture(int id);
    static void SetMaxTextures(uint32_t maxTextures);
    static inline int GetMaxTextures() { return __maxTextures; }
    static bool UsingLargeBindlessTextures() { return __maxTextures == VENOM_MAX_BINDLESS_TEXTURES; }

private:
    virtual void __UpdateDescriptor(const SetsIndex index, const int binding, const void * data, const size_t size, const size_t offset = 0) = 0;
    virtual void __UpdateDescriptor(const SetsIndex index, const int binding, vc::Texture * texture) = 0;

private:
    static int __maxTextures;
};
}
}
