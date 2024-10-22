///
/// Project: VenomEngineWorkspace
/// @file ShaderPipeline.h
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPluginObject.h>

namespace venom
{
namespace common
{
class ShaderResource : public GraphicsCachedResource
{
};

class ShaderImpl : public PluginObjectImpl, public GraphicsPluginObject, public GraphicsCachedResourceHolder
{
public:
    ShaderImpl();
    virtual ~ShaderImpl() = default;
};
}
}