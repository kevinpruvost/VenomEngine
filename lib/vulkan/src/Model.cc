///
/// Project: VenomEngine
/// @file Model.cc
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/Model.h>

namespace venom
{
namespace vulkan
{
VulkanModel::VulkanModel()
    : vc::ModelImpl()
{
    _resource.reset(new VulkanModelResource());
}

VulkanModel::~VulkanModel()
{
}

void VulkanModel::Draw()
{
}
}
}