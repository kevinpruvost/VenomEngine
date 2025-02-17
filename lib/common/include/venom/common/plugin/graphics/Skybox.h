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
#include <venom/common/plugin/graphics/ShaderPipeline.h>

#include <venom/common/ECS.h>

namespace venom
{
namespace common
{

// Panoramic Skyboxes are always 2/1 ratio
#define SKYBOX_IRRADIANCE_HEIGHT 128
#define SKYBOX_IRRADIANCE_WIDTH (SKYBOX_IRRADIANCE_HEIGHT * 2)

#define SKYBOX_RADIANCE_HEIGHT 1024
#define SKYBOX_RADIANCE_WIDTH SKYBOX_RADIANCE_HEIGHT
#define SKYBOX_RADIANCE_MIP_LEVELS 10

struct SkyboxShaderData
{
    float peakLuminance;
    float averageLuminance;
    float blurFactor;
};

class VENOM_COMMON_API SkyboxImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    SkyboxImpl();
    virtual ~SkyboxImpl();

    vc::Error LoadSkybox(const char * texturePath);
    vc::Error LoadSkybox(const SPtr<GraphicsCachedResource> res);
    inline const vc::Texture & GetPanorama() const { return __panorama; }
    inline vc::Texture & GetPanoramaMut() { return __panorama; }
    vc::Error ChangeBlurFactor(const float factor);
protected:
    virtual vc::Error _LoadSkybox(const Texture & texture) = 0;
    virtual vc::Error _LoadIrradianceMap(const Texture & texture, vc::Texture & irradianceMap) = 0;
    virtual vc::Error _LoadRadianceMap(const Texture & texture, Texture & radianceMap) = 0;
    virtual vc::Error _LoadBlurMap(const Texture & texture, Texture & blurMap) = 0;
    virtual vc::Error _ChangeBlurFactor(const float factor) = 0;
protected:
    SkyboxShaderData _shaderData;
private:
    Texture __panorama;
    Texture __radianceMap;
    Texture __irradianceMap;
    Texture __blurMap;

    friend class Skybox;
};

class VENOM_COMMON_API Skybox : public Component, public PluginObjectImplWrapper
{
public:
    Skybox();
    Skybox(const char * path);
    ~Skybox();

    void Init(Entity entity) override;
    void Update(Entity entity) override;
    void _GUI(const Entity entity) override;
    vc::String _GetComponentTitle() override;
    bool CanRemove(Entity entity) override;

    inline vc::Error LoadSkybox(const char * path) { return _impl->As<SkyboxImpl>()->LoadSkybox(path); }
    inline vc::Error LoadSkybox(const SPtr<GraphicsCachedResource> res) { return _impl->As<SkyboxImpl>()->LoadSkybox(res); }
    inline vc::Texture & GetPanoramaMut() { return _impl->As<SkyboxImpl>()->GetPanoramaMut(); }
    inline vc::Error ChangeBlurFactor(const float factor) { return _impl->As<SkyboxImpl>()->ChangeBlurFactor(factor); }
};
}
}
