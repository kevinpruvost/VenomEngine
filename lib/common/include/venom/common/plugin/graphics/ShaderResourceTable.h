///
/// Project: VenomEngineWorkspace
/// @file ShaderResourceTable.h
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/math/Matrix.h>
#include <venom/common/VenomSettings.h>

// @brief Means that all model matrices will be located in a packed buffer
#define VENOM_EXTERNAL_PACKED_MODEL_MATRIX

namespace venom
{
namespace common
{
class ShaderResourceTable : public GraphicsPluginObject
{
public:

#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    static vcm::Mat4 * GetAllModelMatrixBuffer();
    static vcm::Mat4 * GetModelMatrixBuffer();
    static inline size_t GetModelMatrixBufferSize() { return VENOM_MAX_ENTITIES * sizeof(vcm::Mat4); }
    static void ReleaseModelMatrixBuffer(const vcm::Mat4 * mat);
    static int GetModelMatrixBufferId(const vcm::Mat4 * mat);
#endif
};
}
}