///
/// Project: VenomEngineWorkspace
/// @file RenderingPipeline.h
/// @date Nov, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/RenderingPipeline.h>

namespace venom
{
namespace metal
{
class MetalRenderingPipeline : public vc::RenderingPipelineImpl
{
public:
    MetalRenderingPipeline();
    ~MetalRenderingPipeline() override = default;
};
}
}