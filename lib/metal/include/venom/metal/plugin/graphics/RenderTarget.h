///
/// Project: VenomEngineWorkspace
/// @file RenderTarget.h
/// @date Dec, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/RenderTarget.h>
#include <venom/metal/plugin/graphics/Texture.h>

namespace venom
{
namespace metal
{
class MetalRenderTarget : public vc::RenderTargetImpl
{
public:
    MetalRenderTarget();

public:
    vc::Error __PrepareRenderTarget() override;

    //Framebuffer __framebuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
};
}
}
