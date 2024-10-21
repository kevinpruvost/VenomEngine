///
/// Project: VenomEngine
/// @file Model.h
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Model.h>
#include <venom/vulkan/UniformBuffer.h>

namespace venom
{
namespace vulkan
{
class VulkanModelResource : public vc::ModelResource
{
};

class VulkanModel : public vc::ModelImpl
{
public:
    VulkanModel();
    virtual ~VulkanModel();

    void Draw() override;
};
}
}