///
/// Project: VenomEngine
/// @file Shader.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/ShaderPipeline.h>

#include <fstream>

#include <venom/common/Resources.h>
#include <venom/common/math/Vector.h>
#include <venom/common/VenomSettings.h>
#include <venom/metal/MetalApplication.h>

#include <venom/common/plugin/graphics/Light.h>

namespace venom::metal
{
MetalShaderResource::MetalShaderResource(vc::GraphicsCachedResourceHolder* h)
    : ShaderResource(h)
    , pipelineType(PipelineType::Graphics)
    , shaderDirty(true)
{
}

MetalShaderResource::~MetalShaderResource()
{
    DestroyShaderModules();
}

void MetalShaderResource::DestroyShaderModules()
{
}

MetalShaderPipeline::MetalShaderPipeline()
{
    _ResetResource();
}

MetalShaderPipeline::~MetalShaderPipeline()
{
}

MetalShaderPipeline::MetalShaderPipeline(MetalShaderPipeline&& other) noexcept
{
    _resource = std::move(other._resource);
}

MetalShaderPipeline& MetalShaderPipeline::operator=(MetalShaderPipeline&& other) noexcept
{
    if (this != &other) {
        _resource = std::move(other._resource);
    }
    return *this;
}

void MetalShaderPipeline::_ResetResource()
{
    _resource.reset(new MetalShaderResource(this));
}

vc::Error MetalShaderPipeline::_LoadShader(const std::string& path)
{
    std::string basePath = vc::Resources::GetShadersFolderPath() + "compiled/";

    // List all files recursively
    for (const auto& entry : std::filesystem::recursive_directory_iterator(basePath))
    {
        // Gets relative path from ShadersFolderPath
        auto relativePath = entry.path().string().substr(basePath.size());
        // Remove extension
        relativePath = relativePath.substr(0, relativePath.find_first_of('.'));

        if (relativePath != path) continue;

        if (entry.is_regular_file())
        {
            std::string shaderPath = entry.path().string();
            std::string shaderName = entry.path().filename().string();
            if (shaderName.ends_with(".spv"))
            {
                vc::Log::Print("Loading shader: %s", shaderPath.c_str());
                _resource->As<MetalShaderResource>()->shaderPaths.emplace_back(std::move(shaderPath));
            }
        }
    }

    if (_resource->As<MetalShaderResource>()->shaderPaths.empty()) return vc::Error::Failure;

    return vc::Error::Success;
}

void MetalShaderPipeline::_SetLineWidth(const float width)
{
    _resource->As<MetalShaderResource>()->shaderDirty = true;
}

void MetalShaderPipeline::_SetMultiSamplingCount(const int samples)
{
    _resource->As<MetalShaderResource>()->shaderDirty = true;
}

void MetalShaderPipeline::_SetDepthTest(const bool enable)
{
    _resource->As<MetalShaderResource>()->shaderDirty = true;
}

void MetalShaderPipeline::_SetDepthWrite(const bool enable)
{
    _resource->As<MetalShaderResource>()->shaderDirty = true;
}

vc::Error MetalShaderPipeline::_OpenShaders()
{
    // Loading every shader
    _resource->As<MetalShaderResource>()->DestroyShaderModules();
    for (int i = 0; i < _resource->As<MetalShaderResource>()->shaderPaths.size(); ++i)
    {
//        if (LoadShader(_resource->As<MetalShaderResource>()->shaderPaths[i], &_resource->As<MetalShaderResource>()->shaderStages[i]) != vc::Error::Success)
//        {
//            vc::Log::Error("Failed to load shader: %s", _resource->As<MetalShaderResource>()->shaderPaths[i].c_str());
//            return vc::Error::Failure;
//        }
    }

    _resource->As<MetalShaderResource>()->shaderDirty = true;
    return vc::Error::Success;
}

vc::Error MetalShaderPipeline::_ReloadShader()
{
    venom_assert(_renderingPipelineType != vc::RenderingPipelineType::None, "Rendering Pipeline Type is not set");

    if (_resource->As<MetalShaderResource>()->shaderDirty == false) return vc::Error::Success;

    _resource->As<MetalShaderResource>()->shaderDirty = false;
    return vc::Error::Success;
}

void MetalShaderPipeline::_AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location,
    const uint32_t offset, const vc::ShaderVertexFormat format)
{
}
}
