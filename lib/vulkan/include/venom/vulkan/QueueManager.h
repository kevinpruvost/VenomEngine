///
/// Project: VenomEngine
/// @file QueueManager.h
/// @date Sep, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>
#include <venom/vulkan/QueueFamily.h>

namespace venom
{
namespace vulkan
{
class Queue
{
public:
    Queue();
    ~Queue();

    void SetQueueFamilyIndex(uint32_t queueFamilyIndex);
    void SetQueueIndex(uint32_t queueIndex);
    void InitVkQueue();
    uint32_t GetQueueFamilyIndex() const;
    uint32_t GetQueueIndex() const;
    VkQueue GetVkQueue() const;

private:
    uint32_t __queueFamilyIndex;
    uint32_t __queueIndex;
    VkQueue __vkQueue;
};

struct QueueManagerSettings
{
    uint32_t graphicsQueueCount;
    float graphicsQueuePriority;
    uint32_t computeQueueCount;
    float computeQueuePriority;
    uint32_t transferQueueCount;
    float transferQueuePriority;
    uint32_t sparseBindingQueueCount;
    float sparseBindingQueuePriority;
    uint32_t protectedQueueCount;
    float protectedQueuePriority;
    uint32_t videoDecodeQueueCount;
    float videoDecodeQueuePriority;
    uint32_t videoEncodeQueueCount;
    float videoEncodeQueuePriority;
    uint32_t presentQueueCount;
    float presentQueuePriority;
};

class QueueManager
{
public:
    QueueManager();
    ~QueueManager();
    // Shouldn't be moved, belongs to VulkanApplication and nothing else
    QueueManager(const QueueManager & other) = delete;
    QueueManager & operator=(const QueueManager & other) = delete;
    QueueManager(QueueManager && other) = delete;
    QueueManager & operator=(QueueManager && other) = delete;

    vc::Error Init();
    void SetQueueManagerSettings(const QueueManagerSettings & settings);
    vc::Error SetLogicalDeviceQueueCreateInfos(const MappedQueueFamilies & queueFamilies, VkDeviceCreateInfo * createInfo);
    static const Queue & GetGraphicsQueue();
    static const Queue & GetComputeQueue();
    static const Queue & GetTransferQueue();
    static const Queue & GetSparseBindingQueue();
    static const Queue & GetProtectedQueue();
    static const Queue & GetVideoDecodeQueue();
    static const Queue & GetVideoEncodeQueue();
    static const Queue & GetPresentQueue();
    static const std::vector<VkDeviceQueueCreateInfo> & GetQueueCreateInfos();
    static std::vector<uint32_t> GetActiveQueueFamilyIndices();

    // Sharing mode
    static VkSharingMode GetGraphicsTransferSharingMode();
    static VkSharingMode GetGraphicsComputeTransferSharingMode();
    static VkSharingMode GetGraphicsComputeSharingMode();

private:
    vc::Error __TryAddQueueCreateInfo(VkDeviceQueueCreateInfo * createInfo, std::vector<float> * queuePriorities, uint32_t queueFamilyIndex,
        uint32_t * queueCount, const float * queuePriority, size_t maxQueueCount,
        const QueueFamilyIndices & queueFamilyIndices, Queue * queue);

private:
    const MappedQueueFamilies * __queueFamilies;
    QueueManagerSettings __settings;
    std::vector<std::vector<float>> __queuePriorities;
    std::vector<VkDeviceQueueCreateInfo> __queueCreateInfos;
    Queue __graphicsQueue;
    Queue __computeQueue;
    Queue __transferQueue;
    Queue __presentQueue;
    Queue __sparseBindingQueue;
    Queue __protectedQueue;
    Queue __videoDecodeQueue;
    Queue __videoEncodeQueue;

    VkSharingMode __graphicsTransferSharingMode;
    VkSharingMode __graphicsComputeTransferSharingMode;
    VkSharingMode __graphicsComputeSharingMode;
};
}
}