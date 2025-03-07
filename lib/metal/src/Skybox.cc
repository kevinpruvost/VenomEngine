///
/// Project: VenomEngineWorkspace
/// @file Skybox.cc
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/Skybox.h>

#include "venom/common/plugin/graphics/RenderingPipeline.h"

namespace venom
{
namespace metal
{
MetalSkybox::MetalSkybox()
{
}

MetalSkybox::~MetalSkybox()
{
}

vc::Error MetalSkybox::_LoadSkybox(const vc::Texture& texture)
{    return vc::Error::Success;
}

vc::Error MetalSkybox::_LoadIrradianceMap(const vc::Texture& texture, vc::Texture & irradianceMap)
{
    return vc::Error::Success;
}

vc::Error MetalSkybox::_LoadRadianceMap(const vc::Texture& texture, vc::Texture & radianceMap)
{
    return vc::Error::Success;
}

vc::Error MetalSkybox::_ChangeBlurFactor(const float factor) { 
    return vc::Error::Success;
}


vc::Error MetalSkybox::_LoadBlurMap(const common::Texture &texture, common::Texture &blurMap) { 
    return vc::Error::Success;
}


}
}
