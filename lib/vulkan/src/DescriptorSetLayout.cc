///
/// Project: VenomEngine
/// @file DescriptorSetLayout.cc
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/DescriptorSetLayout.h>

#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom
{
namespace vulkan
{
DescriptorSetLayout::DescriptorSetLayout()
    : __layout(VK_NULL_HANDLE)
    , __descriptorSetLayoutInfo()
{
    __descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    if (__layout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(LogicalDevice::GetVkDevice(), __layout, Allocator::GetVKAllocationCallbacks());
    }
}

DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& other) noexcept
    : __layout(std::move(other.__layout))
    , __descriptorSetLayoutInfo(std::move(other.__descriptorSetLayoutInfo))
    , __bindings(std::move(other.__bindings))
{
}

DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout&& other) noexcept
{
    if (this != &other) {
        __layout = std::move(other.__layout);
        __descriptorSetLayoutInfo = std::move(other.__descriptorSetLayoutInfo);
        __bindings = std::move(other.__bindings);
    }
    return *this;
}

void DescriptorSetLayout::AddBinding(uint32_t binding, VkDescriptorType type, uint32_t count,
                                     VkShaderStageFlags stageFlags, const VkSampler* immutableSamplers)
{
    __bindings.push_back({ binding, type, count, stageFlags, immutableSamplers });
    __descriptorSetLayoutInfo.bindingCount = __bindings.size();
    __descriptorSetLayoutInfo.pBindings = __bindings.data();
}

vc::Error DescriptorSetLayout::Create(VkDescriptorSetLayoutCreateFlags flags)
{
    __descriptorSetLayoutInfo.flags = flags;
    if (auto err = vkCreateDescriptorSetLayout(LogicalDevice::GetVkDevice(), &__descriptorSetLayoutInfo, Allocator::GetVKAllocationCallbacks(), &__layout); err != VK_SUCCESS) {
        vc::Log::Error("Failed to create descriptor set layout: %d", err);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

const VkDescriptorSetLayout & DescriptorSetLayout::GetLayout() const
{
    return __layout;
}
}
}