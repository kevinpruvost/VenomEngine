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
    Text2D = 0,
    Text3D = 1,
    BasicModel = 2,
    ShadowModel = 3,
    Skybox = 4,
    Count = 5
};
}
}