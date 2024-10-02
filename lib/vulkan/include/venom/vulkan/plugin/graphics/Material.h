///
/// Project: VenomEngine
/// @file Material.h
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Material.h>

namespace venom
{
namespace vulkan
{

class VulkanMaterial : public vc::Material
{
public:
    VulkanMaterial();
    ~VulkanMaterial();
};

}
}