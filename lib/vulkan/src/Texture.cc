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
#include <venom/vulkan/CommandPool.h>
#include <venom/vulkan/CommandPoolManager.h>
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
    if (CreateImageView().Create(GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
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

    if (CreateImageView().Create(GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
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
    if (CreateImageView().Create(GetImage(), VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT,
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
    if (CreateImageView().Create(GetImage(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
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
    if (CreateImageView().Create(GetImage(), vkFormat, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, imageCount) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::_CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipLevels, int arrayLayers)
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
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, width, height, arrayLayers, mipLevels) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_GENERAL);
    for (int i = 0; i < arrayLayers; ++i) {
        if (CreateImageView().Create(GetImage(), vkFormat, VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_VIEW_TYPE_2D, 0, mipLevels, i, 1) != vc::Error::Success)
            return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error VulkanTexture::_CreateShadowMaps(int dimension)
{
    VkFormat vkFormat = VK_FORMAT_D16_UNORM;

    if (GetImage().Create(vkFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, dimension, dimension, 1, 1) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT);
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_GENERAL);
    if (CreateImageView().Create(GetImage(), vkFormat, GetImage().GetAspectMask(),
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::_CreateShadowCubeMaps(int dimension)
{
    VkFormat vkFormat = VK_FORMAT_D16_UNORM;

    if (GetImage().Create(vkFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, dimension, dimension, 6, 1, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT);
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    for (int i = 0; i < 6; ++i) {
        if (CreateImageView().Create(GetImage(), vkFormat, GetImage().GetAspectMask(),
            VK_IMAGE_VIEW_TYPE_2D, 0, 1, i, 1) != vc::Error::Success)
            return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error VulkanTexture::_SaveImageToFile(const char* path)
{
    VkImageLayout originalLayout = GetImage().GetLayout();
    VkFormat format = GetImage().GetFormat();

    // Transition image layout for transfer
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

    // Create staging buffer
    Buffer stagingBuffer;
    size_t pixelSize = 0;
    switch (format) {
        case VK_FORMAT_R8G8B8A8_SRGB:
            pixelSize = sizeof(unsigned char) * 4;
            break;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            pixelSize = sizeof(uint16_t) * 4;
            break;
        default:
            vc::Log::Error("Unsupported format for saving image to file");
            return vc::Error::Failure;
    };
    vc::Error err = stagingBuffer.CreateBuffer(GetWidth() * GetHeight() * pixelSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (err != vc::Error::Success) return err;

    // Copy to buffer
    {
        SingleTimeCommandBuffer commandBuffer;
        if (CommandPoolManager::GetTransferCommandPool()->CreateSingleTimeCommandBuffer(commandBuffer) != vc::Error::Success)
        {
            vc::Log::Error("Failed to create single time command buffer for generating irradiance map");
            return vc::Error::Failure;
        }
        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { static_cast<uint32_t>(GetWidth()), static_cast<uint32_t>(GetHeight()), 1 };
        
        vkCmdCopyImageToBuffer(
                               commandBuffer,
                               GetImage().GetVkImage(),
                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               stagingBuffer.GetVkBuffer(),
                               1,
                               &region
                               );
    }

    void * data;
    vkMapMemory(LogicalDevice::GetVkDevice(), stagingBuffer.GetVkDeviceMemory(), 0, stagingBuffer.GetSize(), 0, &data);

    // Write image to file
    switch (format)
    {
        case VK_FORMAT_R8G8B8A8_SRGB:
        {
            vc::SaveImageToPng(data, path, GetWidth(), GetHeight(), 4);
            break;
        }
        case VK_FORMAT_R16G16B16A16_SFLOAT:
        {
            vc::SaveImageToExr(data, path, GetWidth(), GetHeight(), 4);
            break;
        }
        default:
            vc::Log::Error("Unsupported format for saving image to file");
            return vc::Error::Failure;
    };

    vkUnmapMemory(LogicalDevice::GetVkDevice(), stagingBuffer.GetVkDeviceMemory());

    // Transition image layout back to original
    GetImage().SetImageLayout(originalLayout);
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
