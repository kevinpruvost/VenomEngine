///
/// Project: VenomEngine
/// @file Sampler.h
/// @date Sep, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class Sampler
{
public:
    Sampler();
    ~Sampler();
    Sampler(const Sampler& other) = delete;
    Sampler& operator=(const Sampler& other) = delete;
    Sampler(Sampler&& other);
    Sampler& operator=(Sampler&& other);

    void SetMagFilter(VkFilter magFilter);
    void SetMinFilter(VkFilter minFilter);
    void SetMipmapMode(VkSamplerMipmapMode mipmapMode);
    void SetAddressModeU(VkSamplerAddressMode addressModeU);
    void SetAddressModeV(VkSamplerAddressMode addressModeV);
    void SetAddressModeW(VkSamplerAddressMode addressModeW);
    void SetMipLodBias(float mipLodBias);
    void SetAnisotropyEnable(VkBool32 anisotropyEnable);
    void SetMaxAnisotropy(float maxAnisotropy);
    void SetCompareEnable(VkBool32 compareEnable);
    void SetCompareOp(VkCompareOp compareOp);
    void SetMinLod(float minLod);
    void SetMaxLod(float maxLod);
    void SetBorderColor(VkBorderColor borderColor);
    void SetUnnormalizedCoordinates(VkBool32 unnormalizedCoordinates);
    void SetCreateInfo(const VkSamplerCreateInfo& createInfo);
    vc::Error Create();

    VkSampler GetSampler() const;
    explicit operator VkSampler() const;
private:
    VkSampler __sampler;
    VkSamplerCreateInfo __createInfo;
};
}
}