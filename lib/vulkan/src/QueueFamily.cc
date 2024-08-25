///
/// Project: VenomEngine
/// @file VulkanQueueFamily.cc
/// @date Aug, 20 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/QueueFamily.h>

namespace venom::vulkan
{
QueueFamily::QueueFamily()
{
}

QueueFamily::~QueueFamily()
{
}

MappedQueueFamilies::MappedQueueFamilies()
{
}

MappedQueueFamilies::MappedQueueFamilies(std::vector<QueueFamily>& queueFamilies)
    : __queueFamilies(std::move(queueFamilies)) // giving ownership of queue families
{
    for (int i = 0; i < __queueFamilies.size(); ++i) {
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            computeQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            transferQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            sparseBindingQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_PROTECTED_BIT) {
            protectedQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) {
            videoDecodeQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) {
            videoEncodeQueueFamilyIndices.push_back(i);
        }
    }
}

MappedQueueFamilies& MappedQueueFamilies::operator=(MappedQueueFamilies&& other) noexcept
{
    __queueFamilies = std::move(other.__queueFamilies);
    graphicsQueueFamilyIndices = std::move(other.graphicsQueueFamilyIndices);
    computeQueueFamilyIndices = std::move(other.computeQueueFamilyIndices);
    transferQueueFamilyIndices = std::move(other.transferQueueFamilyIndices);
    sparseBindingQueueFamilyIndices = std::move(other.sparseBindingQueueFamilyIndices);
    protectedQueueFamilyIndices = std::move(other.protectedQueueFamilyIndices);
    videoDecodeQueueFamilyIndices = std::move(other.videoDecodeQueueFamilyIndices);
    videoEncodeQueueFamilyIndices = std::move(other.videoEncodeQueueFamilyIndices);
    return *this;
}

const std::vector<QueueFamily> MappedQueueFamilies::GetQueueFamilies() const
{
    return __queueFamilies;
}

vc::Error MappedQueueFamilies::SetPresentQueueFamilyIndices(const PhysicalDevice& physicalDevice, const Surface & surface)
{
    for (int i = 0; i < __queueFamilies.size(); ++i) {
        VkBool32 presentSupport = false;
        if (auto err = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice.physicalDevice, i, surface.surface, &presentSupport); err != VK_SUCCESS) {
            vc::Log::Error("Failed to get physical device surface support, error code: %d", err);
            return vc::Error::InitializationFailed;
        }
        if (presentSupport) {
            presentQueueFamilyIndices.push_back(i);
        }
    }
    return vc::Error::Success;
}

MappedQueueFamilies getVulkanQueueFamilies(const PhysicalDevice& physicalDevice)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.physicalDevice, &queueFamilyCount, queueFamilies.data());

    std::vector<QueueFamily> families(queueFamilyCount);
    for (size_t i = 0; i < queueFamilyCount; i++) {
        families[i].properties = queueFamilies[i];
    }
    return MappedQueueFamilies(families);
}
}
