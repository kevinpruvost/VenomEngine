///
/// Project: VenomEngine
/// @file DescriptorPool.cc
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/DescriptorPool.h>

#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/LogicalDevice.h>

namespace venom
{
namespace vulkan
{
DescriptorPool::DescriptorPool()
    : __pool(VK_NULL_HANDLE)
    , __poolInfo()
{
    __poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
}

DescriptorPool::~DescriptorPool()
{
    if (__pool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(LogicalDevice::GetVkDevice(), __pool, Allocator::GetVKAllocationCallbacks());
    }
}

void DescriptorPool::AddPoolSize(VkDescriptorType type, uint32_t count)
{
    __poolSizes.push_back({type, count});
    __poolInfo.poolSizeCount = __poolSizes.size();
    __poolInfo.pPoolSizes = __poolSizes.data();
}

vc::Error DescriptorPool::Create(VkDescriptorPoolCreateFlags flags, uint32_t maxSets)
{
    __poolInfo.flags = flags;
    __poolInfo.maxSets = maxSets;
    if (auto err = vkCreateDescriptorPool(LogicalDevice::GetVkDevice(), &__poolInfo, Allocator::GetVKAllocationCallbacks(), &__pool); err != VK_SUCCESS) {
        vc::Log::Error("Failed to create descriptor pool: %d", err);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

std::vector<DescriptorSet> DescriptorPool::AllocateSets(const VkDescriptorSetLayout& layout, uint32_t count) const { return AllocateSets({count, layout}); }
std::vector<DescriptorSet> DescriptorPool::AllocateSets(const std::vector<VkDescriptorSetLayout>& layouts) const
{
    std::vector<DescriptorSet> sets(layouts.size());
    std::vector<VkDescriptorSet> vkSets(layouts.size());

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = __pool;
    allocInfo.descriptorSetCount = layouts.size();
    allocInfo.pSetLayouts = layouts.data();

    if (auto err = vkAllocateDescriptorSets(LogicalDevice::GetVkDevice(), &allocInfo, reinterpret_cast<VkDescriptorSet*>(vkSets.data())); err != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate descriptor sets: %d", err);
        return sets;
    }
    for (size_t i = 0; i < sets.size(); ++i) { sets[i].__set = vkSets[i]; }
    return sets;
}
}
}
