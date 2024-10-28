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

class ShaderResourceTable : public GraphicsPluginObject
{
public:
    ShaderResourceTable();
    ~ShaderResourceTable() override;

    enum SetsIndex
    {
        SETS_INDEX_MODEL_MATRICES = 0,
        SETS_INDEX_CAMERA = 1,
        SETS_INDEX_TEXTURES = 2,
        SETS_INDEX_SAMPLER = 3,
        SETS_INDEX_MATERIAL = 4,
        SETS_INDEX_SCENE = 5,
    };

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
    static int __maxTextures;
};
}
}