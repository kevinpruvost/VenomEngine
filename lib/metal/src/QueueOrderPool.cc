///
/// Project: VenomEngineWorkspace
/// @file QueueOrderPool.cc
/// @date Feb, 05 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/QueueOrderPool.h>

namespace venom
{
namespace vulkan
{
QueueOrderPool::QueueOrderPool()
{
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i)
    {
        __queueSubmitThreads.emplace_back([this, i]() {
            while (!__stop)
            {
                vc::UniqueLock lock(__mutex[i]);
                __cond[i].wait(lock, [this, i]() { return !__queueOrders[i].empty() || __stop; });

                if (__stop) return;

                QueueOrderInfo orderInfo = __queueOrders[i].front();
                __queueOrders[i].pop();
                lock.unlock();

                // Process order
                switch (orderInfo.type)
                {
                case QueueOrderInfoType_Submit: {
                    const auto & variantInfo = std::get<QueueSubmissionInfo>(orderInfo.info);
                    VkSubmitInfo submitInfo{};
                    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(variantInfo.submitInfo.waitSemaphores.size());
                    submitInfo.pWaitSemaphores = variantInfo.submitInfo.waitSemaphores.data();
                    submitInfo.pWaitDstStageMask = variantInfo.submitInfo.waitDstStageMasks.data();
                    submitInfo.commandBufferCount = static_cast<uint32_t>(variantInfo.submitInfo.commandBuffers.size());
                    submitInfo.pCommandBuffers = variantInfo.submitInfo.commandBuffers.data();
                    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(variantInfo.submitInfo.signalSemaphores.size());
                    submitInfo.pSignalSemaphores = variantInfo.submitInfo.signalSemaphores.data();

                    vkQueueSubmit(variantInfo.queue, 1, &submitInfo, variantInfo.fence);
                    break;
                }
                case QueueOrderInfoType_Present: {
                    const auto & variantInfo = std::get<QueuePresentInfo>(orderInfo.info);
                    VkPresentInfoKHR presentInfo{};
                    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                    presentInfo.waitSemaphoreCount = static_cast<uint32_t>(variantInfo.presentInfo.waitSemaphores.size());
                    presentInfo.pWaitSemaphores = variantInfo.presentInfo.waitSemaphores.data();
                    presentInfo.swapchainCount = static_cast<uint32_t>(variantInfo.presentInfo.swapchains.size());
                    presentInfo.pSwapchains = variantInfo.presentInfo.swapchains.data();
                    presentInfo.pImageIndices = variantInfo.presentInfo.imageIndices.data();

                    vkQueuePresentKHR(variantInfo.queue, &presentInfo);
                    break;
                }
                }
            }
        });
    }
}

QueueOrderPool::~QueueOrderPool()
{
    __stop = true;
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i)
    {
        __cond[i].notify_all();
        __queueSubmitThreads[i].join();
    }
}

void QueueOrderPool::AddQueueOrder(int frameIndex, const QueueOrderInfo& info)
{
    __queueOrders[frameIndex].push(info);
    __cond[frameIndex].notify_one();
}
}
}