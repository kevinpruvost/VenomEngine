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
    for (auto pNext : __createInfoPNexts) {
        delete pNext;
    }
    for (auto flags : __createInfoFlags) {
        delete flags;
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
    __descriptorSetLayoutInfo.pNext = nullptr;
}

vc::Error DescriptorSetLayout::Create()
{
    if (auto err = vkCreateDescriptorSetLayout(LogicalDevice::GetVkDevice(), &__descriptorSetLayoutInfo, Allocator::GetVKAllocationCallbacks(), &__layout); err != VK_SUCCESS) {
        vc::Log::Error("Failed to create descriptor set layout: %d", err);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

bool DescriptorSetLayout::IsBindless() const
{
    for (const auto pNext: __createInfoPNexts) {
        if (*static_cast<VkStructureType *>(pNext) == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT) {
            VkDescriptorSetLayoutBindingFlagsCreateInfoEXT * bindingFlagsCreateInfoExt = reinterpret_cast<VkDescriptorSetLayoutBindingFlagsCreateInfoEXT *>(pNext);
            if (*bindingFlagsCreateInfoExt->pBindingFlags & (VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT | VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT)) {
                return true;
            }
        }
    }
    return false;
}

void DescriptorSetLayout::SetBindingFlags(VkDescriptorBindingFlags flags)
{
    VkDescriptorBindingFlags * allocatedFlags = reinterpret_cast<VkDescriptorBindingFlags *>(__createInfoFlags.emplace_back(new VkDescriptorBindingFlags{flags}));
    VkDescriptorSetLayoutBindingFlagsCreateInfoEXT * bindingFlagsCreateInfoExt = reinterpret_cast<VkDescriptorSetLayoutBindingFlagsCreateInfoEXT *>(__createInfoPNexts.emplace_back(new VkDescriptorSetLayoutBindingFlagsCreateInfoEXT{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT, nullptr, 1, allocatedFlags }));

    if (__descriptorSetLayoutInfo.pNext) {
        vc::Log::Error("DescriptorSetLayoutCreateInfo already has a pNext value, chain not implemented");
        exit(1);
    }
    __descriptorSetLayoutInfo.pNext = bindingFlagsCreateInfoExt;
}

const VkDescriptorSetLayout & DescriptorSetLayout::GetLayout() const
{
    return __layout;
}
}
}