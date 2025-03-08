///
/// Project: VenomEngineWorkspace
/// @file RenderPass.cc
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/RenderPass.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

namespace venom
{
namespace common
{

static vc::Array<RenderPassImpl *, static_cast<int>(vc::RenderingPipelineType::Count)> s_renderPasses{nullptr};

RenderPassImpl::RenderPassImpl()
    : _type(vc::RenderingPipelineType::None)
{
}

RenderPassImpl::~RenderPassImpl()
{
    if (_type != vc::RenderingPipelineType::None) {
        for (int i = 0; i < s_renderPasses.size(); ++i) {
            if (s_renderPasses[i] == this) {
                s_renderPasses[i] = nullptr;
            }
        }
    }
}

RenderPassImpl::RenderPassImpl(RenderPassImpl&& other)
    : _type(other._type)
{
    other._type = vc::RenderingPipelineType::None;

    s_renderPasses[static_cast<int>(_type)] = this;
}

RenderPassImpl& RenderPassImpl::operator=(RenderPassImpl&& other)
{
    if (this != &other) {
        _type = other._type;
        other._type = vc::RenderingPipelineType::None;

        s_renderPasses[static_cast<int>(_type)] = this;
    }
    return *this;
}

void RenderPassImpl::Destroy()
{
    GraphicsPluginObject::Destroy();
}

vc::Error RenderPassImpl::Init()
{
    Destroy();
    return _Init();
}

void RenderPassImpl::SetRenderingType(const vc::RenderingPipelineType type)
{
    _type = type;
    switch (_type) {
        case vc::RenderingPipelineType::Skybox: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Skybox)] = this;
            break;
        }
        case vc::RenderingPipelineType::PBRModel: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::PBRModel)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Reflection)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::AdditiveLighting)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::AdditiveLightingMS)] = this;
            break;
        }
        case vc::RenderingPipelineType::GUI: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::GUI)] = this;
            break;
        }
        case vc::RenderingPipelineType::CascadedShadowMapping: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::CascadedShadowMapping)] = this;
            break;
        }
        case vc::RenderingPipelineType::Text3D: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Text3D)] = this;
            break;
        }
        default: {
            vc::Log::Error("Rendering Pipeline Type not supported");
            break;
        }
    }
}

RenderPassImpl * RenderPassImpl::GetRenderPass(const vc::RenderingPipelineType type)
{
    venom_assert(type != vc::RenderingPipelineType::None && type != vc::RenderingPipelineType::Count, "VulkanRenderPass::GetRenderPass() : type is None or Count");
    return s_renderPasses[static_cast<int>(type)];
}

vc::Array<RenderPassImpl *, static_cast<int>(vc::RenderingPipelineType::Count)> RenderPassImpl::GetRenderPasses()
{
    return s_renderPasses;
}

RenderingPipelineType RenderPassImpl::GetRenderingType() const
{
    return _type;
}

RenderPass::RenderPass()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateRenderPass())
{
}

RenderPass::~RenderPass()
{
}
}
}