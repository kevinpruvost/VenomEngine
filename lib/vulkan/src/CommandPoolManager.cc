///
/// Project: Bazel_Vulkan_Metal
/// @file CommandPoolManager.cc
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/CommandPoolManager.h>

namespace venom
{
namespace vulkan
{
static CommandPoolManager * s_commandPoolManager = nullptr;
CommandPoolManager::CommandPoolManager()
{
    s_commandPoolManager = this;
}

CommandPoolManager::~CommandPoolManager()
{
    s_commandPoolManager = nullptr;
}

vc::Error CommandPoolManager::Init(const MappedQueueFamilies& queueFamilies)
{
    __queueFamilies = &queueFamilies;
    __commandPools.resize(queueFamilies.GetQueueFamilies().size());
    for (int i = 0; i < __commandPools.size(); ++i) {
        __commandPools[i].Init(i);
    }
    return vc::Error::Success;
}

CommandPool* CommandPoolManager::GetGraphicsCommandPool()
{
    CommandPool * commandPool = GetCommandPool(s_commandPoolManager->__queueFamilies->graphicsQueueFamilyIndices);
    if (commandPool == nullptr)
        vc::Log::Error("No graphics command pool found");
    return commandPool;
}

CommandPool* CommandPoolManager::GetCommandPool(const QueueFamilyIndices& queueFamilyIndices)
{
    for (size_t i = 0; i < queueFamilyIndices.size(); ++i) {
        if (s_commandPoolManager->__commandPools[queueFamilyIndices[i]].IsReady()) {
            return &s_commandPoolManager->__commandPools[queueFamilyIndices[i]];
        }
    }
    return nullptr;
}
}
}