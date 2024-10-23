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
#include <venom/vulkan/VulkanApplication.h>

namespace venom
{
namespace vulkan
{
DescriptorPool * DescriptorPool::s_pool = nullptr;

#define GROUP_UPDATE(func) for (int i = 0; i < this->size(); ++i) { this->operator[](i).func; }

void DescriptorSetGroup::GroupUpdate(const VkWriteDescriptorSet& write)
{
    GROUP_UPDATE(Update(write));
}

void DescriptorSetGroup::GroupUpdateBuffer(UniformBuffer& buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    GROUP_UPDATE(UpdateBuffer(buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement));
}

void DescriptorSetGroup::GroupUpdateBuffer(StorageBuffer& buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    GROUP_UPDATE(UpdateBuffer(buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement));
}

void DescriptorSetGroup::GroupUpdateTexture(const VulkanTexture* texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    GROUP_UPDATE(UpdateTexture(texture, binding, descriptorType, descriptorCount, arrayElement));
}

void DescriptorSetGroup::GroupUpdateSampler(const Sampler& sampler, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    GROUP_UPDATE(UpdateSampler(sampler, binding, descriptorType, descriptorCount, arrayElement));
}

DescriptorPool::DescriptorPool()
    : __pool(VK_NULL_HANDLE)
    , __poolInfo()
{
    __poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    s_pool = this;
}

DescriptorPool::~DescriptorPool()
{
    if (__pool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(LogicalDevice::GetVkDevice(), __pool, Allocator::GetVKAllocationCallbacks());
    }
    s_pool = nullptr;
}

void DescriptorPool::AddPoolSize(VkDescriptorType type, uint32_t count)
{
    __poolSizes.push_back({type, count});
    __poolInfo.poolSizeCount = __poolSizes.size();
    __poolInfo.pPoolSizes = __poolSizes.data();
}

void DescriptorPool::AddDescriptorSetLayoutBinding(const uint32_t descriptorSetIndex, const uint32_t binding,
    const VkDescriptorType type, const uint32_t count, const VkShaderStageFlags stageFlags)
{
    // Add the descriptor set layout to the descriptor set layout vector
    while (__descriptorSetLayouts.size() <= descriptorSetIndex)
        __descriptorSetLayouts.emplace_back();
    __descriptorSetLayouts[descriptorSetIndex].AddBinding(binding, type, count, stageFlags);
    AddPoolSize(type, VENOM_MAX_FRAMES_IN_FLIGHT * count);
}

void DescriptorPool::SetDescriptorSetLayoutCreateFlags(const uint32_t descriptorSetIndex, const VkDescriptorSetLayoutCreateFlags flags)
{
    venom_assert(descriptorSetIndex < __descriptorSetLayouts.size(), "Descriptor set index out of range");
    __descriptorSetLayouts[descriptorSetIndex].SetFlags(flags);
}

void DescriptorPool::SetDescriptorSetLayoutBindingSpecifications(const uint32_t descriptorSetIndex, const VkDescriptorBindingFlags flags)
{
    venom_assert(descriptorSetIndex < __descriptorSetLayouts.size(), "Descriptor set index out of range");
    __descriptorSetLayouts[descriptorSetIndex].SetBindingFlags(flags);
}

vc::Error DescriptorPool::Create(VkDescriptorPoolCreateFlags flags, uint32_t maxSets)
{
    // Create Pool
    __poolInfo.flags = flags;
    __poolInfo.maxSets = 0;
    for (auto& poolSize : __poolSizes) {
        __poolInfo.maxSets += poolSize.descriptorCount;
    }
    if (auto err = vkCreateDescriptorPool(LogicalDevice::GetVkDevice(), &__poolInfo, Allocator::GetVKAllocationCallbacks(), &__pool); err != VK_SUCCESS) {
        vc::Log::Error("Failed to create descriptor pool: %d", err);
        return vc::Error::Failure;
    }

    // Create Descriptor Set Layouts
    __vkDescriptorSetLayouts.reserve(__descriptorSetLayouts.size());
    for (auto& descriptorSetLayout : __descriptorSetLayouts)
    {
        if (descriptorSetLayout.Create() != vc::Error::Success)
        {
            vc::Log::Error("Failed to create descriptor set layout");
            return vc::Error::Failure;
        }
        __vkDescriptorSetLayouts.emplace_back(descriptorSetLayout.GetLayout());
    }

    // Allocate Descriptor Sets
    __descriptorSets.reserve(__descriptorSetLayouts.size());
    for (int i = 0; i < __descriptorSetLayouts.size(); ++i) {
        __descriptorSets.emplace_back(AllocateSets(__vkDescriptorSetLayouts[i], maxSets, __descriptorSetLayouts[i].IsBindless()));
    }
    return vc::Error::Success;
}

DescriptorSetGroup DescriptorPool::AllocateSets(const VkDescriptorSetLayout& layout, uint32_t count, const bool bindless) const { return AllocateSets({count, layout}, bindless); }
DescriptorSetGroup DescriptorPool::AllocateSets(const std::vector<VkDescriptorSetLayout>& layouts, const bool bindless) const
{
    DescriptorSetGroup sets(layouts.size());
    std::vector<VkDescriptorSet> vkSets(layouts.size());

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = __pool;
    allocInfo.descriptorSetCount = layouts.size();
    allocInfo.pSetLayouts = layouts.data();

    VkDescriptorSetVariableDescriptorCountAllocateInfoEXT count_info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT };
    std::vector<uint32_t> maxBindings(layouts.size(), vc::ShaderResourceTable::GetMaxTextures());
    if (bindless) {
        count_info.descriptorSetCount = layouts.size();
        // This number is the max allocatable count
        count_info.pDescriptorCounts = maxBindings.data();
        allocInfo.pNext = &count_info;
    }

    if (auto err = vkAllocateDescriptorSets(LogicalDevice::GetVkDevice(), &allocInfo, vkSets.data()); err != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate descriptor sets: %x", err);
        return sets;
    }
    for (size_t i = 0; i < sets.size(); ++i) { sets[i].__set = vkSets[i]; }
    return sets;
}

void DescriptorPool::BindDescriptorSets(const CommandBuffer& commandBuffer, const ShaderPipeline & pipeline, const VkPipelineBindPoint bindPoint)
{
    const int currentFrame = VulkanApplication::GetCurrentFrame();
    std::vector<VkDescriptorSet> descriptorSets;
    descriptorSets.reserve(__descriptorSets.size());
    for (int i = 0; i < __descriptorSets.size(); ++i) {
        descriptorSets.emplace_back(__descriptorSets[i][currentFrame].GetVkDescriptorSet());
    }
    vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(), bindPoint,
        pipeline.GetPipelineLayout(), 0, descriptorSets.size(),
        descriptorSets.data(),
        0, nullptr);
}

void DescriptorPool::BindDescriptorSets(const CommandBuffer& commandBuffer, const ShaderPipeline & pipeline, const VkPipelineBindPoint bindPoint,
    const std::vector<uint32_t>& dynamicOffsets)
{
    const int currentFrame = VulkanApplication::GetCurrentFrame();
    std::vector<VkDescriptorSet> descriptorSets;
    descriptorSets.reserve(__descriptorSets.size());
    for (int i = 0; i < __descriptorSets.size(); ++i) {
        descriptorSets.emplace_back(__descriptorSets[i][currentFrame].GetVkDescriptorSet());
    }
    vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(), bindPoint,
        pipeline.GetPipelineLayout(), 0, descriptorSets.size(),
        descriptorSets.data(),
        dynamicOffsets.size(), dynamicOffsets.data());
}
}
}
