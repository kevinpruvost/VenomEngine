///
/// Project: VenomEngine
/// @file Model.h
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Model.h>

namespace venom
{
namespace vulkan
{
class VulkanModel : public vc::Model
{
public:
    VulkanModel();
    virtual ~VulkanModel();

    void Draw() override;
};
}
}