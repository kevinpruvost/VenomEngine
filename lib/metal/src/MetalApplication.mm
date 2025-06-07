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

@implementation MetalApplicationData

@end

namespace venom::metal
{
MetalApplication::MetalApplication()
    : vc::GraphicsApplication()
    , DebugApplication()
    , __shouldClose(false)
    , __metalApplicationData([[MetalApplicationData alloc] init])
{
    InitializeDevice();
    InitializeCommandQueue();
    InitializeLayer();
    [GetMetalApplicationData() setCommandBuffers:[[NSMutableArray alloc] init]];
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; i++)
        [GetMetalApplicationData().commandBuffers addObject:[[NSMutableArray alloc] init]];
}

MetalApplication::~MetalApplication()
{
    vc::Log::Print("Destroying Metal app...");
    DestroyLayer();
    DestroyCommandQueue();
    DestroyDevice();
#if defined(VENOM_DEBUG)
#endif
    vc::Log::Print("Metal app succesfully destroyed.");
}

NSMutableArray<NSMutableArray<id<MTLCommandBuffer>> *> * MetalApplication::GetCommandBuffers()
{
    return GetMetalApplicationData().commandBuffers;
}

MetalApplicationData * MetalApplication::GetMetalApplicationData()
{
    return (MetalApplicationData *)__metalApplicationData;
}


bool MetalApplication::ShouldClose() { return vc::Context::Get()->ShouldClose(); }

void MetalApplication::PreClose()
{
}

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

    WaitForCurrentCommandBuffersToComplete();

    static const vcm::Vec2 triangleVerticesPos[] =
    {
        // 2D positions
        {  2.0,  -1.0 },
        { -2.0,  -1.0 },
        {  0.0,   3.0 },
    };
    
    static const vcm::Vec4 triangleVerticesColor[] =
    {
        // RGBA colors
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0,  1.0f },
    };
    
    id <MTLCommandBuffer> commandBuffer = CreateCommandBuffer();
    
    MTLRenderPassDescriptor * renderPassDescriptor = MetalRenderPass::GetMetalRenderPass(vc::RenderingPipelineType::Skybox)->GetMetalRenderPassDescriptor();
    id <MTLRenderPipelineState> renderPipelineState = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::Skybox)[0].GetImpl()->As<MetalShaderPipeline>()->GetRenderPipelineState();
    id <CAMetalDrawable> nextFrame = [getGlobalMetalLayer() nextDrawable];
    renderPassDescriptor.colorAttachments[0].texture = nextFrame.texture;
    auto test = nextFrame.texture.sampleCount;

    // Create a render command encoder.
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"Render Encoder 1";
    
    // Set Viewport
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, (double)vc::Context::GetWindowWidth(), (double)vc::Context::GetWindowHeight(), 0.0, 1.0}];
    
    [renderEncoder setRenderPipelineState:renderPipelineState];

    [renderEncoder setVertexBytes:triangleVerticesPos length:sizeof(triangleVerticesPos) atIndex:0];
    [renderEncoder setVertexBytes:triangleVerticesColor length:sizeof(triangleVerticesColor) atIndex:1];
    const vector_uint2 viewPortSize = {(unsigned int)vc::Context::GetWindowWidth(), (unsigned int)vc::Context::GetWindowHeight()};
    [renderEncoder setVertexBytes:&viewPortSize length:sizeof(viewPortSize) atIndex:2];

    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
    
    [renderEncoder endEncoding];
    
    [commandBuffer presentDrawable:nextFrame];
    [commandBuffer commit];
    __shouldClose = vc::Context::Get()->ShouldClose();
    _currentFrame = (_currentFrame + 1) % VENOM_MAX_FRAMES_IN_FLIGHT;
    return err;
}

vc::Error MetalApplication::_OnGfxSettingsChange()
{
    vc::Error err;
    
    // Wait for frames to finish to avoid issues
    WaitForAllCommandBuffersToComplete();

    // If the multisampling is dirty, we need to recreate render pass and shaders
    if (_multisamplingDirty || _hdrDirty)
    {
        // Shaders
        static vc::ShaderPipeline vkShader;
            for (const auto & [key, shader] : vc::ShaderPipelineImpl::GetCachedObjects()) {
                if (!shader->IsType<MetalShaderResource>()) continue;
                if (_multisamplingDirty)
                    shader->GetHolder()->As<MetalShaderPipeline>()->SetMultiSamplingCount(GetActiveSamplesMultisampling());
                shader->GetHolder()->As<MetalShaderPipeline>()->LoadShaders();
            }
        
        // Render Passes
        for (vc::RenderPassImpl * renderPass : vc::RenderPassImpl::GetRenderPasses()) {
            if (renderPass)
                renderPass->SetMultiSampling(GetActiveMultisamplingMode(), GetActiveMultisamplingCount());
        }
        
        // GUI
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

vc::Error MetalApplication::_OnGfxConstantsChange()
{
    if (_windowSizeDirty) {
        getGlobalMetalLayer().drawableSize = CGSizeMake(vc::Context::GetWindowWidth(), vc::Context::GetWindowHeight());
        _windowSizeDirty = false;
    }
    return vc::Error::Success;
}

vc::Error MetalApplication::_SetHDR(bool enable) {
    if (enable) {
        getGlobalMetalLayer().pixelFormat = MTLPixelFormatRGBA16Float;
        getGlobalMetalLayer().wantsExtendedDynamicRangeContent = YES;
    } else {
        getGlobalMetalLayer().pixelFormat = MTLPixelFormatBGRA8Unorm;
        getGlobalMetalLayer().wantsExtendedDynamicRangeContent = NO;
    }
    return vc::Error::Success;
}


vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption> MetalApplication::_GetAvailableMultisamplingOptions() {
    return vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption>();
}


vc::Error MetalApplication::_SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples)
{
    CAMetalLayer * layer = getGlobalMetalLayer();
    switch (mode) {
        case MultiSamplingModeOption::None: {
            layer.framebufferOnly = YES;
            break;
        }
        case MultiSamplingModeOption::MSAA: {
            layer.framebufferOnly = NO;
            break;
        }
        default:
            break;
    }
    return vc::Error::Success;
}

id<MTLCommandBuffer> MetalApplication::CreateCommandBuffer()
{
    id<MTLCommandBuffer> buff = [GetMetalCommandQueue() commandBuffer];
    buff.label = [NSString stringWithFormat:@"Command Buffer (flight %d)", _currentFrame];
    [GetMetalApplicationData().commandBuffers[_currentFrame] addObject:buff];
    return buff;
}

void MetalApplication::WaitForCurrentCommandBuffersToComplete()
{
    NSMutableArray<id<MTLCommandBuffer>> * commandBuffers = GetMetalApplicationData().commandBuffers[_currentFrame];
    for (int i = 0; i < commandBuffers.count; i++)
        [commandBuffers[i] waitUntilCompleted];
    [commandBuffers removeAllObjects];
}

void MetalApplication::WaitForAllCommandBuffersToComplete()
{
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; i++) {
        NSMutableArray<id<MTLCommandBuffer>> * commandBuffers = GetMetalApplicationData().commandBuffers[i];
        for (int j = 0; j < commandBuffers.count; j++)
            [commandBuffers[j] waitUntilCompleted];
        [commandBuffers removeAllObjects];
    }
    _currentFrame = 0;
}

}

