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
namespace vulkan
{
class VulkanRenderingPipeline : public vc::RenderingPipelineImpl
{
public:
    VulkanRenderingPipeline();
    ~VulkanRenderingPipeline() override = default;
};
}
}