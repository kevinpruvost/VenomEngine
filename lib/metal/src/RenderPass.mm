///
/// Project: VenomEngineWorkspace
/// @file RenderPass.mm
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/RenderPass.h>

@implementation MetalRenderPassData
{
@public
}
@end

namespace venom
{
namespace metal
{
MetalRenderPass::MetalRenderPass()
    : renderPassData([[MetalRenderPassData alloc] init])
{
}

MetalRenderPass::~MetalRenderPass()
{
}

MetalRenderPass::MetalRenderPass(MetalRenderPass&& other)
    : RenderPassImpl(std::move(other))
    , renderPassData(other.renderPassData)
{
    other.renderPassData = nullptr;
}

MetalRenderPass& MetalRenderPass::operator=(MetalRenderPass&& other)
{
    if (this != &other)
    {
        renderPassData = other.renderPassData;
        other.renderPassData = nullptr;
        RenderPassImpl::operator=(std::move(other));
    }
    return *this;
}

MTLRenderPassDescriptor * MetalRenderPass::GetMetalRenderPassDescriptor() const {
    return GetMetalRenderPassData().renderPassDescriptor;
}

MetalRenderPassData * MetalRenderPass::GetMetalRenderPassData() const {
    return ((MetalRenderPassData *)renderPassData);
}

vc::Error MetalRenderPass::_Init()
{
    vc::Error err = vc::Error::Success;
    switch (_type) {
        case vc::RenderingPipelineType::Skybox: {
            GetMetalRenderPassData().renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
            MTLRenderPassDescriptor * renderPassDescriptor = GetMetalRenderPassData().renderPassDescriptor;

            // Configure the render pass descriptor for the skybox
            renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
            renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
            renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
            renderPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
            renderPassDescriptor.depthAttachment.clearDepth = 1.0;
            renderPassDescriptor.depthAttachment.storeAction = MTLStoreActionStore;
            break;
        }
        case vc::RenderingPipelineType::PBRModel: {
            break;
        }
        case vc::RenderingPipelineType::GUI:
        break;
        case vc::RenderingPipelineType::CascadedShadowMapping:
        break;
        case vc::RenderingPipelineType::Text3D: {
            vc::Log::Error("Text3D are not supported yet");
            return vc::Error::Failure;
        }
        default: {
            vc::Log::Error("Rendering Pipeline Type not supported");
            return vc::Error::Failure;
        }
    }

    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to create render pass");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}
}
}
