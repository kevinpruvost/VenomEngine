///
/// Project: VenomEngine
/// @file QueueFamily.h
/// @date Aug, 20 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/Surface.h>

namespace venom
{
namespace vulkan
{
class QueueFamily
{
public:
    QueueFamily();
    ~QueueFamily();

public:

public:
    VkQueueFamilyProperties properties;
};
typedef uint32_t QueueFamilyIndex;
typedef vc::Vector<uint32_t> QueueFamilyIndices;

class MappedQueueFamilies
{
public:
    MappedQueueFamilies();

    explicit MappedQueueFamilies(vc::Vector<QueueFamily>& queueFamilies);
    MappedQueueFamilies & operator=(MappedQueueFamilies&& other) noexcept;
    const vc::Vector<QueueFamily> & GetQueueFamilies() const;
    vc::Error SetPresentQueueFamilyIndices(const PhysicalDevice& physicalDevice, const Surface & surface);

public:
    QueueFamilyIndices graphicsQueueFamilyIndices,
    computeQueueFamilyIndices, transferQueueFamilyIndices, sparseBindingQueueFamilyIndices,
    protectedQueueFamilyIndices, videoDecodeQueueFamilyIndices,
    videoEncodeQueueFamilyIndices;
    // To set considering the utilized surface
    QueueFamilyIndices presentQueueFamilyIndices;

private:
    vc::Vector<QueueFamily> __queueFamilies;
};

MappedQueueFamilies getVulkanQueueFamilies(const PhysicalDevice& physicalDevice);
}
}