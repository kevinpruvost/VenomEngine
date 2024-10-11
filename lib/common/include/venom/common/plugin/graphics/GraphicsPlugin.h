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

class Material;
class Model;
class Mesh;
class Texture;
class Camera;

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
    virtual Material* CreateMaterial() = 0;
    virtual Model * CreateModel() = 0;
    virtual Mesh * CreateMesh() = 0;
    virtual Texture * CreateTexture() = 0;
    virtual Camera * CreateCamera() = 0;
};

}
}
