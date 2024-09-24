///
/// Project: VenomEngine
/// @file DescriptorSet.h
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

#include "UniformBuffer.h"

namespace venom
{
namespace vulkan
{
class DescriptorPool;
class DescriptorSet
{
public:
    DescriptorSet();
    ~DescriptorSet();
    DescriptorSet(const DescriptorSet &other) = default;
    DescriptorSet &operator=(const DescriptorSet &other) = default;
    DescriptorSet(DescriptorSet &&other) noexcept = default;
    DescriptorSet &operator=(DescriptorSet &&other) noexcept = default;

    void Update(const VkWriteDescriptorSet &write);
    void Update(UniformBuffer & buffer, uint32_t bufferOffset, uint32_t bufferRange, uint32_t binding,
        VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);

    const VkDescriptorSet & GetVkDescriptorSet() const;
private:
    VkDescriptorSet __set;
};
}
}
