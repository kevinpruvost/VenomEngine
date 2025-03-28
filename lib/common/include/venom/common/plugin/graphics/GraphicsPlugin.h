///
/// Project: VenomEngine
/// @file GraphicsPlugin.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/Plugin.h>
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>

namespace venom
{
namespace common
{

class GraphicsApplication;

class MaterialImpl;
class ModelImpl;
class MeshImpl;
class TextureImpl;
class CameraImpl;
class ShaderPipelineImpl;
class RenderingPipelineImpl;
class SkyboxImpl;
class RenderTargetImpl;
class LightImpl;
class RenderPassImpl;
class BufferImpl;

class ShaderResourceTable;
class GUI;

class VENOM_COMMON_API GraphicsPlugin : public Plugin
{
public:
    enum class GraphicsPluginType
    {
        Vulkan,
#ifdef __APPLE__
        Metal,
#elif defined(_WIN32)
        DirectX12
#endif
    };

public:
    GraphicsPlugin();
    ~GraphicsPlugin() override;

    static GraphicsPlugin * Get();

    virtual GraphicsApplication * CreateGraphicsApplication(int argc, const char* argv[]) = 0;
    
    virtual void TerminatePluginObjects() override;

    // Graphics objects
    virtual MaterialImpl * CreateMaterial() = 0;
    virtual ModelImpl * CreateModel() = 0;
    virtual MeshImpl * CreateMesh() = 0;
    virtual TextureImpl * CreateTexture() = 0;
    virtual CameraImpl * CreateCamera() = 0;
    virtual ShaderPipelineImpl * CreateShaderPipeline() = 0;
    virtual RenderingPipelineImpl * CreateRenderingPipeline() = 0;
    virtual RenderPassImpl * CreateRenderPass() = 0;
    virtual SkyboxImpl * CreateSkybox() = 0;
    virtual RenderTargetImpl * CreateRenderTarget() = 0;
    virtual LightImpl * CreateLight() = 0;
    virtual BufferImpl * CreateBuffer() = 0;

    // Global instances
    virtual ShaderResourceTable * CreateShaderResourceTable() = 0;
    virtual GUI * CreateGUI() = 0;

private:
    friend class GraphicsCachedResource;
    friend class GraphicsPluginObject;
    static inline vc::UMap<vc::String, vc::SPtr<GraphicsCachedResource>> * __GetGraphicsResourceCache() { return Get()->__graphicsResourceCache.get(); }
    vc::UPtr<vc::UMap<vc::String, vc::SPtr<GraphicsCachedResource>>> __graphicsResourceCache;
};

}
}
