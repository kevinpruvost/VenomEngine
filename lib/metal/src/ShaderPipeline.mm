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
#include <venom/metal/Device.h>
#include <venom/metal/Layer.h>

#include <venom/common/plugin/graphics/Light.h>

@implementation MetalShaderPipelineData
{
    @public
    id <MTLFunction> computeShader;
    id <MTLFunction> vertexShader;
    id <MTLFunction> fragmentShader;
    id <MTLRenderPipelineState> renderPipelineState;
    id <MTLComputePipelineState> computePipelineState;
}
@end

namespace venom::metal
{
MetalShaderPipelineData * MetalShaderPipeline::GetPipelineData()
{
    return (MetalShaderPipelineData *)pipelineData;
}

const MetalShaderPipelineData * MetalShaderPipeline::GetPipelineData() const
{
    return (const MetalShaderPipelineData *)pipelineData;
}

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
    : pipelineData([[MetalShaderPipelineData alloc] init])
{
    _ResetResource();
}

id <MTLRenderPipelineState> MetalShaderPipeline::GetRenderPipelineState() const
{
    return GetPipelineData()->renderPipelineState;
}

id <MTLComputePipelineState> MetalShaderPipeline::GetComputePipelineState() const
{
    return GetPipelineData()->computePipelineState;
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

vc::Error MetalShaderPipeline::LoadShader(const std::string& path)
{
    NSError *error = nil;
    // Get the current working directory
    NSString *cwd = [[NSFileManager defaultManager] currentDirectoryPath];
    // Append the relative path to the current working directory
    NSString *fullPath = [cwd stringByAppendingPathComponent:[NSString stringWithUTF8String: path.c_str()]];
    NSURL *fileURL = [NSURL fileURLWithPath:fullPath];

//    NSString *shaderSource = [NSString stringWithContentsOfFile:fullPath encoding:NSUTF8StringEncoding error:&error];
//    if (error) {
//        NSLog(@"Error loading shader file: %@", error.localizedDescription);
//        return vc::Error::Failure;
//    }

//    MTLCompileOptions *options = [[MTLCompileOptions alloc] init];
//    options.fastMathEnabled = YES;
//    options.optimizationLevel = MTLLibraryOptimizationLevelSize;

    id<MTLLibrary> library = [GetMetalDevice() newLibraryWithURL:fileURL error:&error];
    if (!library) {
        NSLog(@"Failed to compile MSL: %@", error);
        return vc::Error::Failure;
    }
    
    // Check if it is a Graphics or Compute Shader
    GetPipelineData()->computeShader = [library newFunctionWithName:@"computeShader"];
    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [MTLRenderPipelineDescriptor new];
    if (GetPipelineData()->computeShader == nil) {
        GetPipelineData()->vertexShader = [library newFunctionWithName:@"vertexShader"];
        GetPipelineData()->fragmentShader = [library newFunctionWithName:@"fragmentShader"];
        if (GetPipelineData()->vertexShader == nil || GetPipelineData()->fragmentShader == nil) {
            return vc::Error::Failure;
        } else {
            pipelineStateDescriptor.label = @"Simple Pipeline";
            pipelineStateDescriptor.vertexFunction = GetPipelineData()->vertexShader;
            pipelineStateDescriptor.fragmentFunction = GetPipelineData()->fragmentShader;
            pipelineStateDescriptor.colorAttachments[0].pixelFormat = GetMetalLayer().pixelFormat;

            GetPipelineData()->renderPipelineState = [GetMetalDevice() newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        }
    } else {
        GetPipelineData()->computePipelineState = [GetMetalDevice() newComputePipelineStateWithFunction:GetPipelineData()->computeShader error:&error];
    }
    return vc::Error::Success;
}

vc::Error MetalShaderPipeline::_LoadShader(const std::string& path)
{
    std::string basePath = vc::Resources::GetShadersFolderPath() + "msl/";

    std::string fullPath = basePath + path + ".metallib";
    // If find full path, then load as library
    if (std::filesystem::exists(fullPath))
    {
        vc::Log::Print("Loading shader: %s", fullPath.c_str());
        _resource->As<MetalShaderResource>()->shaderPaths.emplace_back(std::move(fullPath));
        return LoadShaders();
    }
    return vc::Error::Failure;
}

vc::Error MetalShaderPipeline::LoadShaders()
{
    if (_OpenShaders() != vc::Error::Success) return vc::Error::Failure;
    return _ReloadShader();
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
    // There should be just one file
    if (_resource->As<MetalShaderResource>()->shaderPaths.empty() || _resource->As<MetalShaderResource>()->shaderPaths.size() > 1) return vc::Error::Failure;
    for (int i = 0; i < _resource->As<MetalShaderResource>()->shaderPaths.size(); ++i)
    {
        if (LoadShader(_resource->As<MetalShaderResource>()->shaderPaths[i]) != vc::Error::Success)
        {
            vc::Log::Error("Failed to load shader: %s", _resource->As<MetalShaderResource>()->shaderPaths[i].c_str());
            return vc::Error::Failure;
        }
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
