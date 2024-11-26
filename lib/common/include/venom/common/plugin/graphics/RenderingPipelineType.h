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
enum class RenderingPipelineType
{
    None = -1,
    GUI = 0,
    Text3D,
    Skybox,
    PBRModel,
    ComputeForwardPlusLightCulling,
    ComputeCascadedShadowMapping,
    Count,
};
}
}