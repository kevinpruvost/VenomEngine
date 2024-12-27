///
/// Project: VenomEngine
/// @file Texture.cc
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <imgui_impl_vulkan.h>
#include <venom/vulkan/plugin/graphics/Texture.h>

#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/PhysicalDevice.h>

#include "venom/common/plugin/graphics/GraphicsSettings.h"

namespace venom
{
namespace vulkan
{
VulkanTextureResource::VulkanTextureResource()
{
}

VulkanTexture::VulkanTexture()
    : TextureImpl()
{
    _ResetResource();
}

VulkanTexture::~VulkanTexture()
{
}

static VulkanTexture * s_dummyTexture = nullptr;
VulkanTexture* VulkanTexture::GetDummyTexture()
{
    return s_dummyTexture;
}

void VulkanTexture::SetDummyTexture(VulkanTexture* texture)
{
    s_dummyTexture = texture;
}

void VulkanTexture::_ResetResource()
{
    _resource.reset(new VulkanTextureResource());
}

vc::Error VulkanTexture::LoadImage(unsigned char* pixels, int width, int height, int channels)
{
    // Load Image
    GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
    if (GetImage().Load(pixels, width, height, channels,
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ) != vc::Error::Success)
        return vc::Error::Failure;

    // Create Image View
    if (GetImageView().Create(GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::LoadImageRGBA(unsigned char* pixels, int width, int height, int channels)
{
    GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
    if (GetImage().Load(pixels, width, height, channels,
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ) != vc::Error::Success)
        return vc::Error::Failure;

    if (GetImageView().Create(GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::LoadImage(uint16_t* pixels, int width, int height, int channels)
{
    // Load Image
    GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
    if (GetImage().Load(pixels, width, height, channels,
        VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ) != vc::Error::Success)
        return vc::Error::Failure;

    // Create Image View
    if (GetImageView().Create(GetImage(), VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::_InitDepthBuffer(int width, int height)
{
    VkFormat depthFormat = PhysicalDevice::FindDepthFormat();
    if (GetImage().Create(depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, width, height) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    if (GetImageView().Create(GetImage(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::_CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format)
{
    VkFormat vkFormat;
    switch (format) {
        case vc::ShaderVertexFormat::Float:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16_SFLOAT : VK_FORMAT_R8_SRGB;
            break;
        case vc::ShaderVertexFormat::Vec2:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16G16_SFLOAT : VK_FORMAT_R8G8_SRGB;
            break;
        case vc::ShaderVertexFormat::Vec3:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16G16B16_SFLOAT : VK_FORMAT_B8G8R8_SRGB;
            break;
        case vc::ShaderVertexFormat::Vec4:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16G16B16A16_SFLOAT : VK_FORMAT_B8G8R8A8_SRGB;
            break;
        default:
            vc::Log::Error("Unsupported format for attachment");
            return vc::Error::Failure;
    };

    if (GetImage().Create(vkFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, width, height, imageCount) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    if (GetImageView().Create(GetImage(), vkFormat, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, imageCount) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::_CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipLevels)
{
    VkFormat vkFormat;
    switch (format) {
        case vc::ShaderVertexFormat::Float:
            vkFormat = VK_FORMAT_R16_SFLOAT;
            break;
        case vc::ShaderVertexFormat::Vec2:
            vkFormat = VK_FORMAT_R16G16_SFLOAT;
            break;
        case vc::ShaderVertexFormat::Vec3:
            vkFormat = VK_FORMAT_R16G16B16_SFLOAT;
            break;
        case vc::ShaderVertexFormat::Vec4:
            vkFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
            break;
        default:
            vc::Log::Error("Unsupported format for read write texture");
            return vc::Error::Failure;
    };

    if (GetImage().Create(vkFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, width, height, 1, mipLevels) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_GENERAL);
    if (GetImageView().Create(GetImage(), vkFormat, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, mipLevels, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::_SetMemoryAccess(const vc::TextureMemoryAccess access)
{
    if (_textureUsage & vc::TextureUsage::Storage) {
        VkImageLayout layout;
        switch (access) {
            case vc::TextureMemoryAccess::ReadOnly:
                layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                break;
            case vc::TextureMemoryAccess::ReadWrite:
                layout = VK_IMAGE_LAYOUT_GENERAL;
                break;
            default:
                vc::Log::Error("Unsupported memory access");
                return vc::Error::Failure;
        };
        GetImage().SetImageLayout(layout);
    }
    return vc::Error::Success;
}

vc::Error VulkanTexture::VulkanGUITexture::_LoadTextureToGUI(vc::TextureImpl* impl, void** ptrToGuiTextureId)
{
    const VulkanTexture * vulkanTexture = impl->ConstAs<VulkanTexture>();
    *ptrToGuiTextureId = ImGui_ImplVulkan_AddTexture(Sampler::GetMainSampler()->GetVkSampler(), vulkanTexture->GetImageView().GetVkImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);//, vulkanTexture->GetImage().GetLayout());
    if (!*ptrToGuiTextureId) {
        vc::Log::Error("Failed to load texture to GUI");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error VulkanTexture::VulkanGUITexture::_UnloadTextureFromGUI(void* guiTextureId)
{
    VkDescriptorSet descriptorSet = static_cast<VkDescriptorSet>(guiTextureId);
    ImGui_ImplVulkan_RemoveTexture(descriptorSet);
    return vc::Error::Success;
}

vc::TextureImpl::GUITexture* VulkanTexture::_NewGuiTextureInstance()
{
    return new VulkanGUITexture();
}

int VulkanTexture::GetHeight() const
{
    return GetImage().GetHeight();
}

int VulkanTexture::GetWidth() const
{
    return GetImage().GetWidth();
}

void VulkanTexture::SetDimensions(int width, int height)
{
    if (width == GetWidth() && height == GetHeight())
        return;

    // TODO: Implement
    // Moves image here to destroy after
    // Image image = std::move(GetImage());
    // ImageView imageView = std::move(GetImageView());
}
}
}
