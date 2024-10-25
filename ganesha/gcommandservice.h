#ifndef SPCGANESHAENGINE_GCOMMANDSERVICE_H
#define SPCGANESHAENGINE_GCOMMANDSERVICE_H

#include <vulkan/vulkan.h>
#include <vector>

#include "gvulkandevice.h"

namespace spcGaneshaEngine {

class GCommandServiceProtocol {
public:

    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual VkCommandBuffer allocateCommandBuffer() = 0;
    virtual void submitCommandBuffer(std::vector<VkCommandBuffer> commandBuffersArray,
                                     std::vector<VkSemaphore> waitSemaphoresArray = std::vector<VkSemaphore>(),
                                     std::vector<VkSemaphore> signalSemaphoresArray = std::vector<VkSemaphore>(),
                                     VkFence fence = VK_NULL_HANDLE,
                                     bool waitIdle = true,
                                     bool freeCommandBuffer = true) = 0;
};

class GCommandService: public GCommandServiceProtocol {
public:
    GCommandService(GVULKANDevice& vulkanDevice);
    virtual ~GCommandService();

    void init() override;
    void destroy() override;
    VkCommandBuffer allocateCommandBuffer() override;
    void submitCommandBuffer(std::vector<VkCommandBuffer> commandBuffersArray,
                             std::vector<VkSemaphore> waitSemaphoresArray = std::vector<VkSemaphore>(),
                             std::vector<VkSemaphore> signalSemaphoresArray = std::vector<VkSemaphore>(),
                             VkFence fence = VK_NULL_HANDLE,
                             bool waitIdle = true,
                             bool freeCommandBuffer = true) override;

private:
    VkCommandPool createCommandPool();
    
    GVULKANDevice& vulkanDevice;
    VkCommandPool commandPool;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCOMMANDSERVICE_H
