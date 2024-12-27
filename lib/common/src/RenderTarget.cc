///
/// Project: VenomEngineWorkspace
/// @file RenderTarget.cc
/// @date Dec, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/RenderTarget.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>

#include "venom/common/Context.h"
#include <venom/common/plugin/graphics/Camera.h>


namespace venom
{
namespace common
{
static vc::Vector<RenderTargetImpl*> s_renderTargets;
RenderTargetImpl::RenderTargetImpl()
    : __width(0)
    , __height(0)
    , __format(ShaderVertexFormat::Vec4)
    , __camera(nullptr)
    , __renderingPipelineType(vc::RenderingPipelineType::None)
{
    s_renderTargets.emplace_back(this);
}

RenderTargetImpl::~RenderTargetImpl()
{
    std::erase(s_renderTargets, this);
}

const Texture* RenderTargetImpl::GetTexture() const
{
    return &__texture[GraphicsApplication::GetCurrentFrameInFlight()];
}

vc::Error RenderTargetImpl::Reset()
{
    __width = vc::GraphicsApplication::GetCurrentExtent().x;
    __height = vc::GraphicsApplication::GetCurrentExtent().y;
    return __ReloadTexture();
}

void RenderTargetImpl::Destroy()
{
    GraphicsPluginObject::Destroy();
}

vc::Error RenderTargetImpl::SetDimensions(int width, int height)
{
    // TODO: Not allowed for now
    // __width = width;
    // __height = height;
    return __ReloadTexture();
}

vc::Error RenderTargetImpl::SetFormat(ShaderVertexFormat format)
{
    __format = format;
    return __ReloadTexture();
}

int RenderTargetImpl::GetWidth() const
{
    return __width;
}

int RenderTargetImpl::GetHeight() const
{
    return __height;
}

ShaderVertexFormat RenderTargetImpl::GetFormat() const
{
    return __format;
}

void RenderTargetImpl::LinkCamera(Camera* camera)
{
    __camera = camera;
}

void RenderTargetImpl::SetRenderingPipelineType(vc::RenderingPipelineType type)
{
    __renderingPipelineType = type;
}

vc::RenderingPipelineType RenderTargetImpl::GetRenderingPipelineType() const
{
    return __renderingPipelineType;
}

const vc::Vector<RenderTargetImpl*>& RenderTargetImpl::GetAllRenderTargets()
{
    return s_renderTargets;
}

vc::Error RenderTargetImpl::__ReloadTexture()
{
    vc::Error err;
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        __texture[i] = Texture();
        err = __texture[i].CreateAttachment(__width, __height, 1, __format);
        if (err != vc::Error::Success) {
            vc::Log::Error("Failed to create attachment");
            return err;
        }
    }
    venom_assert(__renderingPipelineType != vc::RenderingPipelineType::None, "RenderingPipelineType is not set");
    if (err = __PrepareRenderTarget(); err != vc::Error::Success) {
        vc::Log::Error("Failed to prepare render target");
        return err;
    }
    return err;
}

RenderTarget::RenderTarget(vc::RenderingPipelineType renderingPipeline, ShaderVertexFormat format)
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateRenderTarget())
{
    GetImpl()->As<RenderTargetImpl>()->__format = format;
    GetImpl()->As<RenderTargetImpl>()->SetRenderingPipelineType(renderingPipeline);
    GetImpl()->As<RenderTargetImpl>()->Reset();
}
}
}
