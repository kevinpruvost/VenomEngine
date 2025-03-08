///
/// Project: VenomEngine
/// @file GraphicsApplication.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/MetalApplication.h>

#include <array>
#include <thread>
#include <vector>

#include <venom/metal/plugin/graphics/ShaderResourceTable.h>

#include <venom/common/FpsCounter.h>

#include <venom/metal/plugin/graphics/Texture.h>

#include <venom/common/VenomSettings.h>
#include <venom/metal/plugin/graphics/Light.h>
#include <venom/metal/plugin/graphics/Material.h>

#include "venom/common/ECS.h"
#include <venom/metal/plugin/graphics/Skybox.h>

#include "venom/common/plugin/graphics/Light.h"
#include "venom/common/SceneSettings.h"
#include "venom/common/plugin/graphics/GUI.h"
#include "venom/common/plugin/graphics/RenderingPipeline.h"
#include "venom/common/plugin/graphics/RenderTarget.h"

#include <venom/metal/Device.h>
#include <venom/metal/CommandQueue.h>
#include <venom/metal/plugin/graphics/RenderPass.h>
#include <venom/metal/Layer.h>
#include <venom/metal/plugin/graphics/Mesh.h>
#include <venom/metal/plugin/graphics/ShaderPipeline.h>

namespace venom::metal
{
MetalApplication::MetalApplication()
    : vc::GraphicsApplication()
    , DebugApplication()
    , __shouldClose(false)
{
    NSApplication * app = [NSApplication sharedApplication];
    InitializeDevice();
    InitializeCommandQueue();
    InitializeLayer();
}

MetalApplication::~MetalApplication()
{
    vc::Log::Print("Destroying Metal app...");
    DestroyLayer();
    DestroyCommandQueue();
    DestroyDevice();
#ifdef VENOM_DEBUG
#endif
    vc::Log::Print("Metal app succesfully destroyed.");
}

bool MetalApplication::ShouldClose() { return vc::Context::Get()->ShouldClose(); }

void MetalApplication::WaitForDraws()
{
}

vc::Error MetalApplication::__Init()
{
    // Check if HDR is possible
    if ([GetMetalDevice() supportsFamily:MTLGPUFamilyApple3]) {
        _isHdrSupported = true;
    }
    
    return vc::Error::Success;
}

vc::Error MetalApplication::__PostInit()
{
    return vc::Error::Success;
}

vc::Error MetalApplication::__Loop()
{
    vc::Error err = vc::Error::Success;

    typedef struct
    {
        vector_float2 position;
        vector_float4 color;
    } AAPLVertex;

    static const AAPLVertex triangleVertices[] =
    {
        // 2D positions,    RGBA colors
        { {  250,  -250 }, { 1, 0, 0, 1 } },
        { { -250,  -250 }, { 0, 1, 0, 1 } },
        { {    0,   250 }, { 0, 0, 1, 1 } },
    };
    
    id <MTLCommandBuffer> commandBuffer = [GetMetalCommandQueue() commandBuffer];
    commandBuffer.label = @"Test1";
    
    MTLRenderPassDescriptor * renderPassDescriptor = MetalRenderPass::GetMetalRenderPass(vc::RenderingPipelineType::Skybox)->GetMetalRenderPassDescriptor();
    id <MTLRenderPipelineState> renderPipelineState = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::Skybox)[0].GetImpl()->As<MetalShaderPipeline>()->GetRenderPipelineState();
    id <CAMetalDrawable> drawable = [GetMetalLayer() nextDrawable];
    renderPassDescriptor.colorAttachments[0].texture = drawable.texture;

    // Create a render command encoder.
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"Render Encoder 1";
    
    // Set Viewport
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, (double)vc::Context::GetWindowWidth(), (double)vc::Context::GetWindowHeight(), 0.0, 1.0}];
    
    [renderEncoder setRenderPipelineState:renderPipelineState];
    
    [renderEncoder setVertexBytes:triangleVertices length:sizeof(triangleVertices) atIndex:0];
    const vector_uint2 viewPortSize = {(unsigned int)vc::Context::GetWindowWidth(), (unsigned int)vc::Context::GetWindowHeight()};
    [renderEncoder setVertexBytes:&viewPortSize length:sizeof(viewPortSize) atIndex:1];

    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
    
    [renderEncoder endEncoding];
    
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
    __shouldClose = vc::Context::Get()->ShouldClose();
    return err;
}

vc::Error MetalApplication::_OnGfxSettingsChange()
{
    vc::Error err;

    if (_windowSizeDirty) {
        GetMetalLayer().drawableSize = CGSizeMake(vc::Context::GetWindowWidth(), vc::Context::GetWindowHeight());
        _windowSizeDirty = false;
    }

    // If the multisampling is dirty, we need to recreate render pass and shaders
    if (_multisamplingDirty || _hdrDirty)
    {
        static vc::ShaderPipeline vkShader;
            for (const auto & [key, shader] : vc::ShaderPipelineImpl::GetCachedObjects()) {
                if (!shader->IsType<MetalShaderResource>()) continue;
                if (_multisamplingDirty)
                    shader->GetHolder()->As<MetalShaderPipeline>()->SetMultiSamplingCount(GetActiveSamplesMultisampling());
                shader->GetHolder()->As<MetalShaderPipeline>()->LoadShaders();
            }
        if (err = vc::GUI::Get()->Reset(); err != vc::Error::Success)
            return err;
        _multisamplingDirty = _hdrDirty = false;
    }

    // Reset Render Targets
    for (auto & rt : vc::RenderTargetImpl::GetAllRenderTargets())
        if (err = rt->Reset(); err != vc::Error::Success)
            return err;
    return err;
}

vc::Error MetalApplication::_SetHDR(bool enable) {
    if (enable) {
        GetMetalLayer().pixelFormat = MTLPixelFormatRGBA16Float;
    } else {
        GetMetalLayer().pixelFormat = MTLPixelFormatBGRA8Unorm;
    }
    return vc::Error::Success;
}


vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption> MetalApplication::_GetAvailableMultisamplingOptions() {
    return vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption>();
}


vc::Error MetalApplication::_SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) {
    return vc::Error::Success;
}
}

