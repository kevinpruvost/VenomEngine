///
/// Project: Bazel_Vulkan_Metal
/// @file PluginObject.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <memory>
#include <venom/common/plugin/Plugin.h>

namespace venom
{
namespace common
{

class PluginObjectImpl
{
};

class VENOM_COMMON_API PluginObject
{
public:
    PluginObject(const Plugin::PluginType type);
    virtual ~PluginObject();
    /// @brief /!\ THIS FUNCTION MUST BE CALLED FOR DESTRUCTION, DO NOT USE `delete`
    void Destroy();

protected:
    std::unique_ptr<PluginObjectImpl> _impl;
private:
    const Plugin::PluginType __type;
};

}
}
