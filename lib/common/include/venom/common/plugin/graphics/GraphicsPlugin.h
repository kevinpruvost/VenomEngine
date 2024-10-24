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

class VENOM_COMMON_API GraphicsPlugin : public Plugin
{
public:
    enum class GraphicsPluginType
    {
        Vulkan,
        Metal,
        DirectX12
    };

public:
    GraphicsPlugin();
    ~GraphicsPlugin() override;

    static GraphicsPlugin * Get();

    virtual GraphicsApplication * CreateGraphicsApplication() = 0;
    virtual MaterialImpl * CreateMaterial() = 0;
    virtual ModelImpl * CreateModel() = 0;
    virtual MeshImpl * CreateMesh() = 0;
    virtual TextureImpl * CreateTexture() = 0;
    virtual CameraImpl * CreateCamera() = 0;

private:
    friend class GraphicsCachedResource;
    friend class GraphicsPluginObject;
    static inline std::unordered_map<std::string, std::shared_ptr<GraphicsCachedResource>> * __GetGraphicsResourceCache() { return Get()->__graphicsResourceCache.get(); }
    std::unique_ptr<std::unordered_map<std::string, std::shared_ptr<GraphicsCachedResource>>> __graphicsResourceCache;
};

}
}
