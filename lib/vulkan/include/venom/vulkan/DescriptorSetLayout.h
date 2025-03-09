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

    DescriptorSetLayout & AddBinding(uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stageFlags, const VkSampler* immutableSamplers = nullptr);
    vc::Error Create();
    inline DescriptorSetLayout & SetBindless() { return SetBindingFlags(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT); }
    inline DescriptorSetLayout & SetFlags(VkDescriptorSetLayoutCreateFlags flags) { __descriptorSetLayoutInfo.flags = flags; return *this; }
    inline VkDescriptorSetLayoutCreateFlags GetFlags() { return __descriptorSetLayoutInfo.flags; }
    bool IsBindless() const;
    DescriptorSetLayout & SetBindingFlags(VkDescriptorBindingFlags flags);
    DescriptorSetLayout & SetMaxSets(uint32_t maxSets);
    uint32_t GetMaxSets() const;
    const vc::Vector<VkDescriptorSetLayoutBinding> & GetBindings() const;

    const VkDescriptorSetLayout & GetLayout() const;

private:
    vc::Vector<void *> __createInfoPNexts, __createInfoFlags;
    vc::Vector<VkDescriptorSetLayoutBinding> __bindings;
    VkDescriptorSetLayoutCreateInfo __descriptorSetLayoutInfo;
    VkDescriptorSetLayout __layout;
    uint32_t __maxSets;
};
}
}