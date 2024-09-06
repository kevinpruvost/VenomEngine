///
/// Project: Bazel_Vulkan_Metal
/// @file QueueManager.cc
/// @date Sep, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/LogicalDevice.h>

#include <algorithm>

#include <venom/vulkan/CommandPoolManager.h>

namespace venom
{
namespace vulkan
{
static QueueManager * s_queueManager = nullptr;

Queue::Queue()
    : __queueFamilyIndex(UINT32_MAX)
    , __queueIndex(UINT32_MAX)
    , __vkQueue(VK_NULL_HANDLE)
{
}

Queue::~Queue()
{
}

void Queue::SetQueueFamilyIndex(uint32_t queueFamilyIndex) { __queueFamilyIndex = queueFamilyIndex; }
void Queue::SetQueueIndex(uint32_t queueIndex) { __queueIndex = queueIndex; }
uint32_t Queue::GetQueueFamilyIndex() const { return __queueFamilyIndex; }
uint32_t Queue::GetQueueIndex() const { return __queueIndex; }
VkQueue Queue::GetVkQueue() const { return __vkQueue; }

void Queue::InitVkQueue()
{
    if (__queueFamilyIndex == UINT32_MAX || __queueIndex == UINT32_MAX) return;
    vkGetDeviceQueue(LogicalDevice::GetVkDevice(), __queueFamilyIndex, __queueIndex, &__vkQueue);
}

QueueManager::QueueManager()
    : __queueFamilies(nullptr)
    , __graphicsQueue{}
    , __computeQueue{}
    , __transferQueue{}
    , __sparseBindingQueue{}
    , __protectedQueue{}
    , __videoDecodeQueue{}
    , __videoEncodeQueue{}
    , __presentQueue{}
    , __queueCreateInfos{}
    , __graphicsComputeSharingMode(VK_SHARING_MODE_EXCLUSIVE)
    , __graphicsComputeTransferSharingMode(VK_SHARING_MODE_EXCLUSIVE)
    , __graphicsTransferSharingMode(VK_SHARING_MODE_EXCLUSIVE)
{
    s_queueManager = this;
}

QueueManager::~QueueManager()
{
    s_queueManager = nullptr;
}

vc::Error QueueManager::Init()
{
    __graphicsQueue.InitVkQueue();
    __computeQueue.InitVkQueue();
    __transferQueue.InitVkQueue();
    __presentQueue.InitVkQueue();
    __sparseBindingQueue.InitVkQueue();
    __protectedQueue.InitVkQueue();
    __videoDecodeQueue.InitVkQueue();
    __videoEncodeQueue.InitVkQueue();

    // Mandatory queues
    CommandPoolManager::GetGraphicsCommandPool()->SetQueue(&__graphicsQueue);
    CommandPoolManager::GetComputeCommandPool()->SetQueue(&__computeQueue);
    CommandPoolManager::GetTransferCommandPool()->SetQueue(&__transferQueue);
    CommandPoolManager::GetPresentCommandPool()->SetQueue(&__presentQueue);
    // Optional queues
    if (CommandPoolManager::GetSparseBindingCommandPool() != nullptr)
       CommandPoolManager::GetSparseBindingCommandPool()->SetQueue(&__sparseBindingQueue);
    if (CommandPoolManager::GetProtectedCommandPool() != nullptr)
        CommandPoolManager::GetProtectedCommandPool()->SetQueue(&__protectedQueue);
    if (CommandPoolManager::GetVideoDecodeCommandPool() != nullptr)
        CommandPoolManager::GetVideoDecodeCommandPool()->SetQueue(&__videoDecodeQueue);
    if (CommandPoolManager::GetVideoEncodeCommandPool() != nullptr)
        CommandPoolManager::GetVideoEncodeCommandPool()->SetQueue(&__videoEncodeQueue);

    // Check for sharing mode
    if (__graphicsQueue.GetQueueFamilyIndex() != __computeQueue.GetQueueFamilyIndex())
    {
        __graphicsComputeSharingMode = VK_SHARING_MODE_CONCURRENT;
        if (__graphicsQueue.GetQueueFamilyIndex() != __transferQueue.GetQueueFamilyIndex())
            __graphicsComputeTransferSharingMode = VK_SHARING_MODE_CONCURRENT;
    }
    if (__graphicsQueue.GetQueueFamilyIndex() != __transferQueue.GetQueueFamilyIndex())
        __graphicsTransferSharingMode = VK_SHARING_MODE_CONCURRENT;
    return vc::Error::Success;
}

void QueueManager::SetQueueManagerSettings(const QueueManagerSettings& settings)
{
    __settings = settings;
}

vc::Error QueueManager::SetLogicalDeviceQueueCreateInfos(const MappedQueueFamilies& queueFamilies,
                                                         VkDeviceCreateInfo* createInfo)
{
    __queueFamilies = &queueFamilies;

    __queueCreateInfos.resize(__queueFamilies->GetQueueFamilies().size(), {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = 0,
        .queueCount = 0,
        .pQueuePriorities = nullptr
    });
    __queuePriorities.resize(__queueCreateInfos.size());
    // Calculate Queues to create for each queue family
    for (size_t i = 0; i < __queueFamilies->GetQueueFamilies().size(); ++i)
    {
        __queueCreateInfos[i].queueFamilyIndex = i;
        size_t maxQueueCount = __queueFamilies->GetQueueFamilies()[i].properties.queueCount;
        // Graphics queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.graphicsQueueCount, &__settings.graphicsQueuePriority, maxQueueCount, __queueFamilies->graphicsQueueFamilyIndices, &__graphicsQueue);
        // Compute queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.computeQueueCount, &__settings.computeQueuePriority, maxQueueCount, __queueFamilies->computeQueueFamilyIndices, &__computeQueue);
        // Present queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.presentQueueCount, &__settings.presentQueuePriority, maxQueueCount, __queueFamilies->presentQueueFamilyIndices, &__presentQueue);
        // Transfer queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.transferQueueCount, &__settings.transferQueuePriority, maxQueueCount, __queueFamilies->transferQueueFamilyIndices, &__transferQueue);
        // Sparse binding queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.sparseBindingQueueCount, &__settings.sparseBindingQueuePriority, maxQueueCount, __queueFamilies->sparseBindingQueueFamilyIndices, &__sparseBindingQueue);
        // Protected queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.protectedQueueCount, &__settings.protectedQueuePriority, maxQueueCount, __queueFamilies->protectedQueueFamilyIndices, &__protectedQueue);
        // Video decode queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.videoDecodeQueueCount, &__settings.videoDecodeQueuePriority, maxQueueCount, __queueFamilies->videoDecodeQueueFamilyIndices, &__videoDecodeQueue);
        // Video encode queues
        __TryAddQueueCreateInfo(&__queueCreateInfos[i], &__queuePriorities[i], i, &__settings.videoEncodeQueueCount, &__settings.videoEncodeQueuePriority, maxQueueCount, __queueFamilies->videoEncodeQueueFamilyIndices, &__videoEncodeQueue);

        // Queue priorities
        __queueCreateInfos[i].pQueuePriorities = __queuePriorities[i].data();
    }
    std::erase_if(__queueCreateInfos, [](const VkDeviceQueueCreateInfo& createInfo) { return createInfo.queueCount == 0; });
    createInfo->pQueueCreateInfos = __queueCreateInfos.data();
    createInfo->queueCreateInfoCount = static_cast<uint32_t>(__queueCreateInfos.size());
    return vc::Error::Success;
}

const Queue & QueueManager::GetGraphicsQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__graphicsQueue;
}

const Queue & QueueManager::GetComputeQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__computeQueue;
}

const Queue & QueueManager::GetTransferQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__transferQueue;
}

const Queue & QueueManager::GetSparseBindingQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__sparseBindingQueue;
}

const Queue & QueueManager::GetProtectedQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__protectedQueue;
}

const Queue & QueueManager::GetVideoDecodeQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__videoDecodeQueue;
}

const Queue & QueueManager::GetVideoEncodeQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__videoEncodeQueue;
}

const Queue & QueueManager::GetPresentQueue()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__presentQueue;
}

const std::vector<VkDeviceQueueCreateInfo>& QueueManager::GetQueueCreateInfos()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    return s_queueManager->__queueCreateInfos;
}

std::vector<uint32_t> QueueManager::GetActiveQueueFamilyIndices()
{
    venom_assert(s_queueManager != nullptr, "QueueManager has not been initialized");
    std::vector<uint32_t> indices;
    indices.reserve(s_queueManager->__queueCreateInfos.size());
    for (const auto& createInfo : s_queueManager->__queueCreateInfos) {
        indices.emplace_back(createInfo.queueFamilyIndex);
    }
    return indices;
}

VkSharingMode QueueManager::GetGraphicsTransferSharingMode()
{
    return s_queueManager->__graphicsTransferSharingMode;
}

VkSharingMode QueueManager::GetGraphicsComputeTransferSharingMode()
{
    return s_queueManager->__graphicsComputeTransferSharingMode;
}

VkSharingMode QueueManager::GetGraphicsComputeSharingMode()
{
    return s_queueManager->__graphicsComputeSharingMode;
}

vc::Error QueueManager::__TryAddQueueCreateInfo(VkDeviceQueueCreateInfo* createInfo, std::vector<float> * queuePriorities,
                                                uint32_t queueFamilyIndex, uint32_t * queueCount, const float * queuePriority,
                                                size_t maxQueueCount, const QueueFamilyIndices & queueFamilyIndices, Queue * queue)
{
    venom_assert(*queueCount <= 1, "More than 1 queue per feature is not supported yet");
    if (*queueCount == 0) return vc::Error::Success;
    if (std::ranges::find(queueFamilyIndices, queueFamilyIndex) != queueFamilyIndices.end()) {
        // Count possible queues to add
        uint32_t possibleQueues = std::min<uint32_t>(*queueCount, maxQueueCount - createInfo->queueCount);
        createInfo->queueCount += possibleQueues;
        *queueCount -= possibleQueues;
        if (possibleQueues == 0)
            return vc::Error::Success;
        for (uint32_t i = 0; i < possibleQueues; ++i) {
            queuePriorities->push_back(*queuePriority);
        }
        // Works only for 1 queue here
        queue->SetQueueIndex(createInfo->queueCount - possibleQueues);
        queue->SetQueueFamilyIndex(queueFamilyIndex);
    }
    return vc::Error::Success;
}
}
}

