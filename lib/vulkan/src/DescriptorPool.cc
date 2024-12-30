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

#define GROUP_UPDATE(func) for (int i = 0; i < __descriptorSets.size(); ++i) { __descriptorSets[i].func; }

DescriptorSetGroup::~DescriptorSetGroup()
{
}

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

void DescriptorSetGroup::GroupUpdateImageView(const ImageView& imageView, uint32_t binding,
    VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    GROUP_UPDATE(UpdateImageView(imageView, binding, descriptorType, descriptorCount, arrayElement));
}

void DescriptorSetGroup::GroupUpdatePerFrame(int frameIndex, const VkWriteDescriptorSet& write)
{
    __descriptorSets[frameIndex].Update(write);
}

void DescriptorSetGroup::GroupUpdateBufferPerFrame(int frameIndex, UniformBuffer& buffer, uint32_t bufferOffset,
    uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    __descriptorSets[frameIndex].UpdateBuffer(buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement);
}

void DescriptorSetGroup::GroupUpdateBufferPerFrame(int frameIndex, StorageBuffer& buffer, uint32_t bufferOffset,
    uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    __descriptorSets[frameIndex].UpdateBuffer(buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement);
}

void DescriptorSetGroup::GroupUpdateTexturePerFrame(int frameIndex, const VulkanTexture* texture, uint32_t binding,
    VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    __descriptorSets[frameIndex].UpdateTexture(texture, binding, descriptorType, descriptorCount, arrayElement);
}

void DescriptorSetGroup::GroupUpdateSamplerPerFrame(int frameIndex, const Sampler& sampler, uint32_t binding,
    VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    __descriptorSets[frameIndex].UpdateSampler(sampler, binding, descriptorType, descriptorCount, arrayElement);
}

void DescriptorSetGroup::GroupUpdateImageViewPerFrame(int frameIndex, const ImageView& imageView, uint32_t binding,
    VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    __descriptorSets[frameIndex].UpdateImageView(imageView, binding, descriptorType, descriptorCount, arrayElement);
}

DescriptorSetGroupAllocator::DescriptorSetGroupAllocator(std::vector<DescriptorSetGroup>&& sets)
    : __descriptorSetGroups(std::move(sets))
{
    for (int i = static_cast<int>(__descriptorSetGroups.size()) - 1; i >= 0; --i) {
        __freeSets.push(&__descriptorSetGroups[i]);
    }
}

DescriptorSetGroup* DescriptorSetGroupAllocator::AllocateSet()
{
    DescriptorSetGroup * set = __freeSets.top();
    __freeSets.pop();
    return set;
}

void DescriptorSetGroupAllocator::FreeSet(DescriptorSetGroup* set)
{
    venom_assert(set, "Freeing nullptr descriptor set");
    __freeSets.push(set);
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
    __poolSizes.emplace_back(type, count);
    __poolInfo.poolSizeCount = static_cast<uint32_t>(__poolSizes.size());
    __poolInfo.pPoolSizes = __poolSizes.data();
}

void DescriptorPool::AddDescriptorSetLayoutBinding(const uint32_t descriptorSetIndex, const uint32_t binding,
    const VkDescriptorType type, const uint32_t descriptorCount, const VkShaderStageFlags stageFlags)
{
    // Add the descriptor set layout to the descriptor set layout vector
    while (__descriptorSetLayouts.size() <= descriptorSetIndex)
        __descriptorSetLayouts.emplace_back();
    __descriptorSetLayouts[descriptorSetIndex].AddBinding(binding, type, descriptorCount, stageFlags);
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

void DescriptorPool::SetDescriptorSetLayoutMaxSets(const uint32_t descriptorSetIndex, const uint32_t maxSets)
{
    venom_assert(descriptorSetIndex < __descriptorSetLayouts.size(), "Descriptor set index out of range");
    __descriptorSetLayouts[descriptorSetIndex].SetMaxSets(maxSets);
}

DescriptorSetLayout& DescriptorPool::GetOrCreateDescriptorSetLayout(const uint32_t descriptorSetIndex)
{
    while (__descriptorSetLayouts.size() <= descriptorSetIndex)
        __descriptorSetLayouts.emplace_back();
    return __descriptorSetLayouts[descriptorSetIndex];
}

vc::Error DescriptorPool::Create(VkDescriptorPoolCreateFlags flags, uint32_t maxSets)
{
    // Create Pool
    __poolInfo.flags = flags;
    __poolInfo.maxSets = 0;
    for (int i = 0; i < __descriptorSetLayouts.size(); ++i) {
        __poolInfo.maxSets += VENOM_MAX_FRAMES_IN_FLIGHT * __descriptorSetLayouts[i].GetMaxSets();
        const auto & bindings = __descriptorSetLayouts[i].GetBindings();
        for (const auto & binding : bindings) {
            AddPoolSize(binding.descriptorType, VENOM_MAX_FRAMES_IN_FLIGHT * __descriptorSetLayouts[i].GetMaxSets() * binding.descriptorCount);
        }
    }
    // For GUI
    __poolInfo.maxSets += VENOM_MAX_FRAMES_IN_FLIGHT * 64;
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
        __descriptorSets.emplace_back(AllocateSets(__vkDescriptorSetLayouts[i], VENOM_MAX_FRAMES_IN_FLIGHT * __descriptorSetLayouts[i].GetMaxSets() , __descriptorSetLayouts[i].IsBindless()));
    }
    return vc::Error::Success;
}

std::vector<DescriptorSetGroup> DescriptorPool::AllocateSets(const VkDescriptorSetLayout& layout, uint32_t count, const bool bindless) const { return AllocateSets({count, layout}, bindless); }
std::vector<DescriptorSetGroup> DescriptorPool::AllocateSets(const std::vector<VkDescriptorSetLayout>& layouts, const bool bindless) const
{
    std::vector<DescriptorSetGroup> sets(layouts.size() / VENOM_MAX_FRAMES_IN_FLIGHT);
    std::vector<VkDescriptorSet> vkSets(layouts.size());

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = __pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
    allocInfo.pSetLayouts = layouts.data();

    VkDescriptorSetVariableDescriptorCountAllocateInfoEXT count_info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT };
    std::vector<uint32_t> maxBindings(layouts.size(), vc::ShaderResourceTable::UsingLargeBindlessTextures() ? vc::ShaderResourceTable::GetMaxTextures() : VENOM_MAX_DYNAMIC_TEXTURES);
    if (bindless) {
        count_info.descriptorSetCount = static_cast<uint32_t>(layouts.size());
        // This number is the max allocatable count
        count_info.pDescriptorCounts = maxBindings.data();
        allocInfo.pNext = &count_info;
    }

    if (auto err = vkAllocateDescriptorSets(LogicalDevice::GetVkDevice(), &allocInfo, vkSets.data()); err != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate descriptor sets: %x", err);
        return sets;
    }
    for (size_t i = 0; i < vkSets.size(); ++i) {
        sets[i / VENOM_MAX_FRAMES_IN_FLIGHT][i % VENOM_MAX_FRAMES_IN_FLIGHT].__set = vkSets[i];
    }
    return sets;
}

void DescriptorPool::BindDescriptorSets(const int descriptorSetIndex, const CommandBuffer& commandBuffer, const VulkanShaderPipeline * pipeline)
{
    venom_assert(descriptorSetIndex < __descriptorSets.size(), "Descriptor set index out of range");
    venom_assert(__descriptorSets[descriptorSetIndex].size(), "Multiple groups here, this function is meant for single group descriptor sets");
    const int currentFrame = VulkanApplication::GetCurrentFrameInFlight();
    const VkPipelineBindPoint bindPoint = pipeline->GetRenderingPipelineShaderType() == common::RenderingPipelineShaderType::Compute ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS;
    vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(), bindPoint,
        pipeline->GetPipelineLayout(), descriptorSetIndex, 1,
        __descriptorSets[descriptorSetIndex][0][currentFrame].GetVkDescriptorSetPtr(),
        0, nullptr);
}

void DescriptorPool::BindDescriptorSets(const int descriptorSetIndex, const CommandBuffer& commandBuffer, const VulkanShaderPipeline * pipeline, const std::vector<uint32_t>& dynamicOffsets)
{
    venom_assert(descriptorSetIndex < __descriptorSets.size(), "Descriptor set index out of range");
    venom_assert(__descriptorSets[descriptorSetIndex].size(), "Multiple groups here, this function is meant for single group descriptor sets");
    const int currentFrame = VulkanApplication::GetCurrentFrameInFlight();
    const VkPipelineBindPoint bindPoint = pipeline->GetRenderingPipelineShaderType() == common::RenderingPipelineShaderType::Compute ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS;
    vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(), bindPoint,
        pipeline->GetPipelineLayout(), descriptorSetIndex, 1,
        __descriptorSets[descriptorSetIndex][0][currentFrame].GetVkDescriptorSetPtr(),
        static_cast<uint32_t>(dynamicOffsets.size()), dynamicOffsets.data());
}

}
}
