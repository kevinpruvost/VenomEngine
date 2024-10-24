///
/// Project: VenomEngine
/// @file DescriptorSetLayout.h
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class DescriptorSetLayout
{
public:
    DescriptorSetLayout();
    ~DescriptorSetLayout();
    DescriptorSetLayout(const DescriptorSetLayout& other) = delete;
    DescriptorSetLayout& operator=(const DescriptorSetLayout& other) = delete;
    DescriptorSetLayout(DescriptorSetLayout&& other) noexcept;
    DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept;

    void AddBinding(uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stageFlags, const VkSampler* immutableSamplers = nullptr);
    vc::Error Create();
    inline void SetFlags(VkDescriptorSetLayoutCreateFlags flags) { __descriptorSetLayoutInfo.flags = flags; }
    inline VkDescriptorSetLayoutCreateFlags GetFlags() { return __descriptorSetLayoutInfo.flags; }
    bool IsBindless() const;
    void SetBindingFlags(VkDescriptorBindingFlags flags);

    const VkDescriptorSetLayout & GetLayout() const;

private:
    std::vector<void *> __createInfoPNexts, __createInfoFlags;
    std::vector<VkDescriptorSetLayoutBinding> __bindings;
    VkDescriptorSetLayoutCreateInfo __descriptorSetLayoutInfo;
    VkDescriptorSetLayout __layout;
};
}
}