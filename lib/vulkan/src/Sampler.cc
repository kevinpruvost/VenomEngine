///
/// Project: VenomEngine
/// @file Sampler.cc
/// @date Sep, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Sampler.h>

#include <venom/vulkan/Instance.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom
{
namespace vulkan
{
Sampler::Sampler()
    : __sampler(VK_NULL_HANDLE)
    , __createInfo({
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias = 0.0f,
        .anisotropyEnable = VK_FALSE,
        .maxAnisotropy = 1.0f,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0f,
        .maxLod = 0.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE
    })
{
}

Sampler::~Sampler()
{
    if (__sampler != VK_NULL_HANDLE) {
        vkDestroySampler(LogicalDevice::GetVkDevice(), __sampler, Allocator::GetVKAllocationCallbacks());
    }
    VkSamplerCreateInfo createInfo;
}

Sampler::Sampler(Sampler&& other)
    : __sampler(other.__sampler)
    , __createInfo(other.__createInfo)
{
    other.__sampler = VK_NULL_HANDLE;
}

Sampler& Sampler::operator=(Sampler&& other)
{
    if (this != &other) {
        __sampler = other.__sampler;
        other.__sampler = VK_NULL_HANDLE;
        __createInfo = other.__createInfo;
    }
    return *this;
}

void Sampler::SetMagFilter(VkFilter magFilter)
{
    __createInfo.magFilter = magFilter;
}

void Sampler::SetMinFilter(VkFilter minFilter)
{
    __createInfo.minFilter = minFilter;
}

void Sampler::SetMipmapMode(VkSamplerMipmapMode mipmapMode)
{
    __createInfo.mipmapMode = mipmapMode;
}

void Sampler::SetAddressModeU(VkSamplerAddressMode addressModeU)
{
    __createInfo.addressModeU = addressModeU;
}

void Sampler::SetAddressModeV(VkSamplerAddressMode addressModeV)
{
    __createInfo.addressModeV = addressModeV;
}

void Sampler::SetAddressModeW(VkSamplerAddressMode addressModeW)
{
    __createInfo.addressModeW = addressModeW;
}

void Sampler::SetMipLodBias(float mipLodBias)
{
    __createInfo.mipLodBias = mipLodBias;
}

void Sampler::SetAnisotropyEnable(VkBool32 anisotropyEnable)
{
    __createInfo.anisotropyEnable = anisotropyEnable;
}

void Sampler::SetMaxAnisotropy(float maxAnisotropy)
{
    __createInfo.maxAnisotropy = maxAnisotropy;
}

void Sampler::SetCompareEnable(VkBool32 compareEnable)
{
    __createInfo.compareEnable = compareEnable;
}

void Sampler::SetCompareOp(VkCompareOp compareOp)
{
    __createInfo.compareOp = compareOp;
}

void Sampler::SetMinLod(float minLod)
{
    __createInfo.minLod = minLod;
}

void Sampler::SetMaxLod(float maxLod)
{
    __createInfo.maxLod = maxLod;
}

void Sampler::SetBorderColor(VkBorderColor borderColor)
{
    __createInfo.borderColor = borderColor;
}

void Sampler::SetUnnormalizedCoordinates(VkBool32 unnormalizedCoordinates)
{
    __createInfo.unnormalizedCoordinates = unnormalizedCoordinates;
}

void Sampler::SetCreateInfo(const VkSamplerCreateInfo& createInfo)
{
    __createInfo = createInfo;
}

vc::Error Sampler::Create()
{
    if (VkResult err = vkCreateSampler(LogicalDevice::GetVkDevice(), &__createInfo, Allocator::GetVKAllocationCallbacks(), &__sampler); err != VK_SUCCESS) {
        vc::Log::Error("Failed to create sampler: %d", err);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

VkSampler Sampler::GetVkSampler() const
{
    return __sampler;
}

Sampler::operator VkSampler() const
{
    return __sampler;
}
}
}