///
/// Project: VenomEngineWorkspace
/// @file ShaderPipeline.cc
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/ShaderPipeline.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/Resources.h>

#include <venom/common/math/Matrix.h>
#include <venom/common/plugin/graphics/GraphicsSettings.h>

#include <filesystem>

namespace venom
{
namespace common
{
ShaderResource::ShaderResource(GraphicsCachedResourceHolder* holder)
    : GraphicsCachedResource(holder)
{
}

ShaderPipelineImpl::ShaderPipelineImpl()
    : _renderingPipelineType(RenderingPipelineType::None)
    , _renderingPipelineShaderType(RenderingPipelineShaderType::None)
    , _renderingPipelineIndex(std::numeric_limits<uint32_t>::max())
    , _loaded(false)
    , __customSamples(false)
{
}

vc::Error ShaderPipelineImpl::LoadShaderFromFile(const vc::String & path)
{
    venom_assert(_renderingPipelineType != RenderingPipelineType::None, "RenderingPipelineType is not set");
    venom_assert(_renderingPipelineShaderType != RenderingPipelineShaderType::None, "RenderingPipelineShaderType is not set");
    // Check if path contains shaders
    {
        // Load from cache if already loaded
        vc::SPtr<GraphicsCachedResource> cachedShader = GraphicsPluginObject::GetCachedObject(path);
        if (cachedShader) {
            _LoadFromCache(cachedShader);
            return vc::Error::Success;
        }
    }

    vc::Error err = _LoadShader(path);
    if (err != Error::Success) {
        vc::Log::Error("Failed to load shader from path [%s]", path.c_str());
        return err;
    }
    // Set In Cache
    _SetInCache(path, _GetResourceToCache());
    return err;
}

void ShaderPipelineImpl::AddVertexBufferToLayout(const ShaderVertexFormat format, const uint32_t binding,
    const uint32_t location, const uint32_t offset)
{
    uint32_t formatSize = 0;
    switch (format)
    {
        case ShaderVertexFormat::Float:
            formatSize = sizeof(float);
            break;
        case ShaderVertexFormat::Vec2:
            formatSize = sizeof(vcm::Vec2);
            break;
        case ShaderVertexFormat::Vec3:
            formatSize = sizeof(vcm::Vec3);
            break;
        case ShaderVertexFormat::Vec4:
            formatSize = sizeof(vcm::Vec4);
            break;
        case ShaderVertexFormat::Int:
            formatSize = sizeof(int);
            break;
        case ShaderVertexFormat::IVec2:
            formatSize = sizeof(vcm::IVec2);
            break;
        case ShaderVertexFormat::IVec3:
            formatSize = sizeof(vcm::IVec3);
            break;
        case ShaderVertexFormat::IVec4:
            formatSize = sizeof(vcm::IVec4);
            break;
        case ShaderVertexFormat::Uint:
            formatSize = sizeof(uint32_t);
            break;
        case ShaderVertexFormat::UVec2:
            formatSize = sizeof(vcm::UVec2);
            break;
        case ShaderVertexFormat::UVec3:
            formatSize = sizeof(vcm::UVec3);
            break;
        case ShaderVertexFormat::UVec4:
            formatSize = sizeof(vcm::UVec4);
            break;
        case ShaderVertexFormat::Mat2:
            formatSize = sizeof(vcm::Mat2);
            break;
        case ShaderVertexFormat::Mat3:
            formatSize = sizeof(vcm::Mat3);
            break;
        case ShaderVertexFormat::Mat4:
            formatSize = sizeof(vcm::Mat4);
            break;
        default:
            venom_assert(false, "Unknown ShaderVertexFormat");
            break;
    }
    _AddVertexBufferToLayout(formatSize, binding, location, offset, format);
}

void ShaderPipelineImpl::AddVertexBufferToLayout(const VertexBufferLayout& layout)
{
    AddVertexBufferToLayout(layout.format, layout.binding, layout.location, layout.offset);
}

void ShaderPipelineImpl::AddVertexBufferToLayout(const vc::Vector<VertexBufferLayout>& layouts)
{
    for (const VertexBufferLayout& layout : layouts) {
        AddVertexBufferToLayout(layout);
    }
}

void ShaderPipelineImpl::SetCustomMultiSamplingCount(const int samples)
{
    __customSamples = true;
    _SetMultiSamplingCount(samples);
}

void ShaderPipelineImpl::SetMultiSamplingCount(const int samples)
{
    if (__customSamples) return;
    _SetMultiSamplingCount(samples);
}

ShaderPipeline::ShaderPipeline()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateShaderPipeline())
{
}

ShaderPipeline::ShaderPipeline(const char* path)
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateShaderPipeline())
{
    if (Error err = LoadShaderFromFile(path); err != Error::Success) {
        Destroy();
        return;
    }
}

ShaderPipeline::~ShaderPipeline()
{
}

vc::Error ShaderPipeline::RecompileAllShaders()
{
    auto oldPath = std::filesystem::current_path();
    std::filesystem::path parentDir = CMAKE_PROJECT_DIR;
    std::filesystem::current_path(parentDir);

    int result = std::system("make compile_shaders");
    std::filesystem::current_path(oldPath);
    if (result != 0) {
        vc::Log::Error("Failed to compile shaders with make");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error ShaderPipeline::ReloadAllShaders()
{
    for (const auto & [key, shader] : vc::ShaderPipelineImpl::GetCachedObjects()) {
        if (!shader->IsType<ShaderResource>()) continue;
        shader->GetHolder()->As<ShaderPipelineImpl>()->OpenAndReloadShader();
    }
    GraphicsSettings::ReloadGFXSettings();
}
}
}
