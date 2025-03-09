///
/// Project: VenomEngine
/// @file CommandPoolManager.h
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/QueueFamily.h>
#include <venom/vulkan/CommandPool.h>

#include <unordered_map>

namespace venom
{
namespace vulkan
{
class CommandPoolManager
{
public:
    CommandPoolManager();
    ~CommandPoolManager();
    CommandPoolManager(const CommandPoolManager & other) = delete;
    CommandPoolManager & operator=(const CommandPoolManager & other) = delete;
    // Shouldn't be moved, belongs to VulkanApplication and nothing else
    CommandPoolManager(CommandPoolManager && other) = delete;
    CommandPoolManager & operator=(CommandPoolManager && other) = delete;

    vc::Error Init();

    static CommandPool * GetGraphicsCommandPool();
    static CommandPool * GetComputeCommandPool();
    static CommandPool * GetTransferCommandPool();
    static CommandPool * GetPresentCommandPool();
    static CommandPool * GetSparseBindingCommandPool();
    static CommandPool * GetProtectedCommandPool();
    static CommandPool * GetVideoDecodeCommandPool();
    static CommandPool * GetVideoEncodeCommandPool();

private:
    static CommandPool * GetCommandPool(const QueueFamilyIndices & queueFamilyIndices);

    CommandPool * __graphicsPool;
    CommandPool * __computePool;
    CommandPool * __transferPool;
    CommandPool * __presentPool;
    CommandPool * __sparseBindingPool;
    CommandPool * __protectedPool;
    CommandPool * __videoDecodePool;
    CommandPool * __videoEncodePool;

private:
    vc::UMap<uint32_t, CommandPool> __commandPools;
};
}
}