///
/// Project: VenomEngineWorkspace
/// @file RenderTarget.h
/// @date Dec, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/Texture.h>
#include <venom/common/plugin/graphics/RenderingPipelineType.h>

namespace venom
{
namespace common
{
class RenderTarget;
class Camera;

class VENOM_COMMON_API RenderTargetImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    RenderTargetImpl();
    ~RenderTargetImpl();

    const Texture * GetTexture() const;
    vc::Error Reset();
    virtual void Destroy() override;

    vc::Error SetDimensions(int width, int height);
    vc::Error SetFormat(ShaderVertexFormat format);

    int GetWidth() const;
    int GetHeight() const;
    ShaderVertexFormat GetFormat() const;

    void LinkCamera(vc::Camera * camera);
    void SetRenderingPipelineType(vc::RenderingPipelineType type);
    vc::RenderingPipelineType GetRenderingPipelineType() const;

    static const vc::Vector<RenderTargetImpl *> & GetAllRenderTargets();

protected:
    virtual vc::Error __PrepareRenderTarget() = 0;

private:
    vc::Error __ReloadTexture();

private:
    Texture __texture[VENOM_MAX_FRAMES_IN_FLIGHT];
    int __width, __height;
    ShaderVertexFormat __format;
    vc::Camera * __camera;
    vc::RenderingPipelineType __renderingPipelineType;

    friend class RenderTarget;
};

class VENOM_COMMON_API RenderTarget : public PluginObjectImplWrapper
{
public:
    RenderTarget(vc::RenderingPipelineType renderingPipeline, ShaderVertexFormat format);

    const Texture * GetTexture() const { return GetImpl()->As<RenderTargetImpl>()->GetTexture(); }
};
}
}