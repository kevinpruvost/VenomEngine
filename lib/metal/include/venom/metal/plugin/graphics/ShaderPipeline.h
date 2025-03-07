///
/// Project: VenomEngine
/// @file ShaderPipeline.h
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/ShaderPipeline.h>

#include <venom/common/math/Matrix.h>

namespace venom
{
namespace metal
{
enum class PipelineType : uint32_t
{
    Graphics = 0,
    Compute = 1,
    RayTracing = 2
};

class MetalShaderResource : public vc::ShaderResource
{
public:
    MetalShaderResource(vc::GraphicsCachedResourceHolder * h);
    ~MetalShaderResource() override;

    void DestroyShaderModules();
public:
    bool shaderDirty;
    PipelineType pipelineType;
};

class MetalShaderPipeline : public vc::ShaderPipelineImpl
{
public:
    MetalShaderPipeline();
    ~MetalShaderPipeline() override;
    MetalShaderPipeline(const MetalShaderPipeline&) = delete;
    MetalShaderPipeline& operator=(const MetalShaderPipeline&) = delete;
    MetalShaderPipeline(MetalShaderPipeline&& other) noexcept;
    MetalShaderPipeline& operator=(MetalShaderPipeline&& other) noexcept;

    void _ResetResource() override;
    vc::Error _LoadShader(const std::string & path) override;
    void _SetMultiSamplingCount(const int samples) override;
    void _SetLineWidth(const float width) override;
    void _SetDepthTest(const bool enable) override;
    void _SetDepthWrite(const bool enable) override;
    vc::Error _OpenShaders() override;
    vc::Error _ReloadShader() override;

    void _AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location, const uint32_t offset, const vc::ShaderVertexFormat format) override;
private:
};

}
}
