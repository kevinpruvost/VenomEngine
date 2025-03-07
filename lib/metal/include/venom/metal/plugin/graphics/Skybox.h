///
/// Project: VenomEngineWorkspace
/// @file Skybox.h
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/Skybox.h>

namespace venom
{
namespace metal
{
class MetalSkybox : public vc::SkyboxImpl
{
public:
    MetalSkybox();
    ~MetalSkybox() override;

    vc::Error _LoadSkybox(const vc::Texture & texture) override;
    vc::Error _LoadIrradianceMap(const vc::Texture& texture, vc::Texture & irradianceMap) override;
    vc::Error _LoadRadianceMap(const vc::Texture& texture, vc::Texture& radianceMap) override;
    vc::Error _LoadBlurMap(const vc::Texture & texture, vc::Texture & blurMap) override;
    vc::Error _ChangeBlurFactor(const float factor) override;
};
}
}
