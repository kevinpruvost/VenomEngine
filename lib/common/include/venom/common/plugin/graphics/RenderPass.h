///
/// Project: VenomEngineWorkspace
/// @file RenderPass.h
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/plugin/graphics/RenderingPipelineType.h>
#include <venom/common/plugin/graphics/Texture.h>
#include <venom/common/plugin/graphics/GraphicsSettings.h>

#include <venom/common/Containers.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API RenderPassImpl : public GraphicsPluginObject
{
public:
    RenderPassImpl();
    ~RenderPassImpl();
    RenderPassImpl(const RenderPassImpl&) = delete;
    RenderPassImpl& operator=(const RenderPassImpl&) = delete;
    RenderPassImpl(RenderPassImpl&& other);
    RenderPassImpl& operator=(RenderPassImpl&& other);

    void Destroy() override;
    inline vc::Error Init() { return _Init(); }
    vc::Error SetMultiSampling(const vc::GraphicsSettings::MultiSamplingModeOption mode, const vc::GraphicsSettings::MultiSamplingCountOption samples);

    void SetRenderingType(const vc::RenderingPipelineType type);
    static RenderPassImpl * GetRenderPass(const vc::RenderingPipelineType type);
    static vc::Array<RenderPassImpl *, static_cast<int>(vc::RenderingPipelineType::Count)> GetRenderPasses();
    RenderingPipelineType GetRenderingType() const;

    virtual vc::Error _Init() = 0;
    virtual vc::Error _SetMultiSampling(const vc::GraphicsSettings::MultiSamplingModeOption mode, const vc::GraphicsSettings::MultiSamplingCountOption samples) = 0;

protected:
    vc::RenderingPipelineType _type;
    vc::Vector<vc::Vector<vc::Texture>> _attachments;
};

class VENOM_COMMON_API RenderPass : public PluginObjectWrapper
{
public:
    RenderPass();
    ~RenderPass();

    inline void SetRenderingType(const vc::RenderingPipelineType type) { _impl->As<RenderPassImpl>()->SetRenderingType(type); }
};
}
}
