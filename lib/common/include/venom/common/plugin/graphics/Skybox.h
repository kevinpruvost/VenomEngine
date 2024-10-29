///
/// Project: VenomEngineWorkspace
/// @file Skybox.h
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/plugin/graphics/Texture.h>
#include <venom/common/plugin/graphics/Shader.h>

namespace venom
{
namespace common
{
class SkyboxImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    SkyboxImpl();
    virtual ~SkyboxImpl() = default;

    vc::Error LoadSkybox(const char * texturePath);
    inline const vc::Texture & GetPanorama() const { return __panorama; }
protected:
    virtual vc::Error _LoadSkybox(const Texture & texture) = 0;
private:
    Texture __panorama;
};

class VENOM_COMMON_API Skybox : public PluginObjectImplWrapper
{
public:
    Skybox();
    Skybox(const char * path);
    ~Skybox();

    inline vc::Error LoadSkybox(const char * path) { return _impl->As<SkyboxImpl>()->LoadSkybox(path); }
};
}
}
