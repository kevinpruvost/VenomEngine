///
/// Project: VenomEngineWorkspace
/// @file RenderingPipelineType.h
/// @date Nov, 12 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

namespace venom
{
namespace common
{
enum class RenderingPipelineShaderType
{
    None = -1,
    Graphics = 0,
    Compute,
    Count,
};

enum class RenderingPipelineType
{
    None = -1,
    GUI = 0,
    Text3D,
    Skybox,
    PBRModel,
    ForwardPlusLightCulling,
    CascadedShadowMapping,
    BRDF_LUT,
    IrradianceMap,
    RadianceMap,
    Count,
};
}
}