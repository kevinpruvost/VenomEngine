///
/// Project: VenomEngineWorkspace
/// @file QueueOrderPool.h
/// @date Feb, 05 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Containers.h>
#include <venom/common/Thread.h>
#include <venom/vulkan/QueueManager.h>
#include <venom/common/VenomSettings.h>

namespace venom
{
namespace vulkan
{
class QueueOrderPool
{
public:
    QueueOrderPool();
    ~QueueOrderPool();

    struct AllocatedSubmitInfo {
        AllocatedSubmitInfo() = default;
        AllocatedSubmitInfo(const AllocatedSubmitInfo& submitInfo)
            : waitSemaphores(submitInfo.waitSemaphores)
            , waitDstStageMasks(submitInfo.waitDstStageMasks)
            , commandBuffers(submitInfo.commandBuffers)
            , signalSemaphores(submitInfo.signalSemaphores)
        {
        }
        AllocatedSubmitInfo & operator=(const AllocatedSubmitInfo& submitInfo)
        {
            waitSemaphores = submitInfo.waitSemaphores;
            waitDstStageMasks = submitInfo.waitDstStageMasks;
            commandBuffers = submitInfo.commandBuffers;
            signalSemaphores = submitInfo.signalSemaphores;
            return *this;
        }
        vc::Vector<VkSemaphore> waitSemaphores;
        vc::Vector<VkPipelineStageFlags> waitDstStageMasks;
        vc::Vector<VkCommandBuffer> commandBuffers;
        vc::Vector<VkSemaphore> signalSemaphores;
    };
    struct QueueSubmissionInfo
    {
        VkQueue queue;
        VkFence fence;
        AllocatedSubmitInfo submitInfo;
    };

    struct AllocatedPresentInfo
    {
        AllocatedPresentInfo() = default;
        AllocatedPresentInfo(const AllocatedPresentInfo& presentInfo)
            : waitSemaphores(presentInfo.waitSemaphores)
            , swapchains(presentInfo.swapchains)
            , imageIndices(presentInfo.imageIndices)
        {
        }
        AllocatedPresentInfo & operator=(const AllocatedPresentInfo& presentInfo)
        {
            waitSemaphores = presentInfo.waitSemaphores;
            swapchains = presentInfo.swapchains;
            imageIndices = presentInfo.imageIndices;
            return *this;
        }
        vc::Vector<VkSemaphore> waitSemaphores;
        vc::Vector<VkSwapchainKHR> swapchains;
        vc::Vector<uint32_t> imageIndices;
    };
    struct QueuePresentInfo
    {
        VkQueue queue;
        AllocatedPresentInfo presentInfo;
    };
    enum QueueOrderInfoType
    {
        QueueOrderInfoType_Submit,
        QueueOrderInfoType_Present
    };
    struct QueueOrderInfo
    {
        QueueOrderInfoType type;
        std::variant<QueueSubmissionInfo, QueuePresentInfo> info;
        // union {
        //     QueueSubmissionInfo submitInfo;
        //     QueuePresentInfo presentInfo;
        // };
    };

    void AddQueueOrder(int frameIndex, const QueueOrderInfo & info);
private:
    vc::Array<vc::Queue<QueueOrderInfo>, VENOM_MAX_FRAMES_IN_FLIGHT> __queueOrders;
    vc::Vector<vc::Thread> __queueSubmitThreads;
    vc::Atomic<bool> __stop;
    vc::Array<vc::ConditionVariable, VENOM_MAX_FRAMES_IN_FLIGHT> __cond;
    vc::Array<vc::Mutex, VENOM_MAX_FRAMES_IN_FLIGHT> __mutex;
};
}
}