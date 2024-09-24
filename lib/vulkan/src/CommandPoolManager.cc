///
/// Project: VenomEngine
/// @file CommandPoolManager.cc
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/CommandPoolManager.h>
#include <venom/vulkan/QueueManager.h>

namespace venom
{
namespace vulkan
{
static CommandPoolManager * s_commandPoolManager = nullptr;
CommandPoolManager::CommandPoolManager()
    : __graphicsPool(nullptr)
    , __computePool(nullptr)
    , __transferPool(nullptr)
    , __presentPool(nullptr)
    , __sparseBindingPool(nullptr)
    , __protectedPool(nullptr)
    , __videoDecodePool(nullptr)
    , __videoEncodePool(nullptr)
{
    s_commandPoolManager = this;
}

CommandPoolManager::~CommandPoolManager()
{
    s_commandPoolManager = nullptr;
}

vc::Error CommandPoolManager::Init()
{
    auto activeQueueFamilies = QueueManager::GetActiveQueueFamilyIndices();

    __commandPools.reserve(activeQueueFamilies.size());
    for (int i = 0; i < activeQueueFamilies.size(); ++i) {
        __commandPools[activeQueueFamilies[i]].Init(activeQueueFamilies[i]);
    }

    uint32_t graphicsQueueFamilyIndex = QueueManager::GetGraphicsQueue().GetQueueFamilyIndex();
    uint32_t computeQueueFamilyIndex = QueueManager::GetComputeQueue().GetQueueFamilyIndex();
    uint32_t transferQueueFamilyIndex = QueueManager::GetTransferQueue().GetQueueFamilyIndex();
    uint32_t presentQueueFamilyIndex = QueueManager::GetPresentQueue().GetQueueFamilyIndex();
    uint32_t sparseBindingQueueFamilyIndex = QueueManager::GetSparseBindingQueue().GetQueueFamilyIndex();
    uint32_t protectedQueueFamilyIndex = QueueManager::GetProtectedQueue().GetQueueFamilyIndex();
    uint32_t videoDecodeQueueFamilyIndex = QueueManager::GetVideoDecodeQueue().GetQueueFamilyIndex();
    uint32_t videoEncodeQueueFamilyIndex = QueueManager::GetVideoEncodeQueue().GetQueueFamilyIndex();

    // If the queue family index is UINT32_MAX, it means that the queue family is not active
    // Not active means that the queue family is not supported by the physical device
    // OR
    // The queue family is not requested by the application or the user
    if (graphicsQueueFamilyIndex != UINT32_MAX)
        __graphicsPool = &__commandPools[graphicsQueueFamilyIndex];
    if (computeQueueFamilyIndex != UINT32_MAX)
        __computePool = &__commandPools[computeQueueFamilyIndex];
    if (transferQueueFamilyIndex != UINT32_MAX)
        __transferPool = &__commandPools[transferQueueFamilyIndex];
    if (presentQueueFamilyIndex != UINT32_MAX)
        __presentPool = &__commandPools[presentQueueFamilyIndex];
    if (sparseBindingQueueFamilyIndex != UINT32_MAX)
        __sparseBindingPool = &__commandPools[sparseBindingQueueFamilyIndex];
    if (protectedQueueFamilyIndex != UINT32_MAX)
        __protectedPool = &__commandPools[protectedQueueFamilyIndex];
    if (videoDecodeQueueFamilyIndex != UINT32_MAX)
        __videoDecodePool = &__commandPools[videoDecodeQueueFamilyIndex];
    if (videoEncodeQueueFamilyIndex != UINT32_MAX)
        __videoEncodePool = &__commandPools[videoEncodeQueueFamilyIndex];

    return vc::Error::Success;
}

CommandPool* CommandPoolManager::GetGraphicsCommandPool() { return s_commandPoolManager->__graphicsPool; }
CommandPool* CommandPoolManager::GetComputeCommandPool() { return s_commandPoolManager->__computePool; }
CommandPool* CommandPoolManager::GetTransferCommandPool() { return s_commandPoolManager->__transferPool; }
CommandPool* CommandPoolManager::GetPresentCommandPool() { return s_commandPoolManager->__presentPool; }
CommandPool* CommandPoolManager::GetSparseBindingCommandPool() { return s_commandPoolManager->__sparseBindingPool; }
CommandPool* CommandPoolManager::GetProtectedCommandPool() { return s_commandPoolManager->__protectedPool; }
CommandPool* CommandPoolManager::GetVideoDecodeCommandPool() { return s_commandPoolManager->__videoDecodePool; }
CommandPool* CommandPoolManager::GetVideoEncodeCommandPool() { return s_commandPoolManager->__videoEncodePool; }
}
}
