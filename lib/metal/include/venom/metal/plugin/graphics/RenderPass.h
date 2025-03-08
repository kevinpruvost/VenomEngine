///
/// Project: VenomEngineWorkspace
/// @file RenderPass.h
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/RenderPass.h>

#ifdef __OBJC__
#include <venom/metal/Device.h>

@interface MetalRenderPassData : NSObject
@end

#endif

namespace venom
{
namespace metal
{
class MetalRenderPass : public vc::RenderPassImpl
{
public:
    MetalRenderPass();
    ~MetalRenderPass();
    MetalRenderPass(const MetalRenderPass&) = delete;
    MetalRenderPass& operator=(const MetalRenderPass&) = delete;
    MetalRenderPass(MetalRenderPass&& other);
    MetalRenderPass& operator=(MetalRenderPass&& other);

    inline static MetalRenderPass * GetMetalRenderPass(const vc::RenderingPipelineType type) { return GetRenderPass(type)->DAs<MetalRenderPass>(); }

    vc::Error _Init() override;
    void * renderPassData;

#ifdef __OBJC__
    MTLRenderPassDescriptor * GetMetalRenderPassDescriptor() const;
    MetalRenderPassData * GetMetalRenderPassData() const;
#endif
};
}
}
