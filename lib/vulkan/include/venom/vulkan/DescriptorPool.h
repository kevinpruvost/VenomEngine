///
/// Project: VenomEngine
/// @file DescriptorPool.h
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/DescriptorSet.h>

namespace venom
{
namespace vulkan
{
class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();
    DescriptorPool &operator=(const DescriptorPool &other) = delete;
    DescriptorPool(const DescriptorPool &other) = delete;
    DescriptorPool &operator=(DescriptorPool &&other) noexcept = delete;
    DescriptorPool(DescriptorPool &&other) noexcept = delete;

    void AddPoolSize(VkDescriptorType type, uint32_t count);
    vc::Error Create(VkDescriptorPoolCreateFlags flags, uint32_t maxSets);
    std::vector<DescriptorSet> AllocateSets(const VkDescriptorSetLayout &layout, uint32_t count) const;
    std::vector<DescriptorSet> AllocateSets(const std::vector<VkDescriptorSetLayout> &layouts) const;

private:
    std::vector<VkDescriptorPoolSize> __poolSizes;
    VkDescriptorPoolCreateInfo __poolInfo;
    VkDescriptorPool __pool;
};
}
}