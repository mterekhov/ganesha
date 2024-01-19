#include "gcommandservice.h"

namespace spcGaneshaEngine {

GCommandService::GCommandService(GVULKANDevice& vulkanDevice) : vulkanDevice(vulkanDevice) {
}

GCommandService::~GCommandService() {
}

#pragma mark - GCommandServiceProtocol -

void GCommandService::init() {
    commandPool = createCommandPool(vulkanDevice);
}

void GCommandService::destroy() {
    vkDestroyCommandPool(vulkanDevice.getLogicalDevice(), commandPool, nullptr);
}

VkCommandBuffer GCommandService::allocateCommandBuffer() {
    VkCommandBufferAllocateInfo allocInfo = { };
    
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer newCommandBuffer;
    vkAllocateCommandBuffers(vulkanDevice.getLogicalDevice(), &allocInfo, &newCommandBuffer);

    return newCommandBuffer;
}

void GCommandService::submitCommandBuffer(std::vector<VkCommandBuffer> commandBuffersArray,
                                          std::vector<VkSemaphore> waitSemaphoresArray,
                                          std::vector<VkSemaphore> signalSemaphoresArray,
                                          VkFence fence,
                                          bool waitIdle,
                                          bool freeCommandBuffer) {
    VkSubmitInfo submitInfo = { };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    if (!waitSemaphoresArray.empty()) {
        submitInfo.waitSemaphoreCount = waitSemaphoresArray.size();
        submitInfo.pWaitSemaphores = waitSemaphoresArray.data();
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.pWaitDstStageMask = waitStages;
    }
    
    if (!signalSemaphoresArray.empty()) {
        submitInfo.signalSemaphoreCount = signalSemaphoresArray.size();
        submitInfo.pSignalSemaphores = signalSemaphoresArray.data();
    }
    
    if (!commandBuffersArray.empty()) {
        submitInfo.commandBufferCount = commandBuffersArray.size();
        submitInfo.pCommandBuffers = commandBuffersArray.data();
    }
        
    if (vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, fence) != VK_SUCCESS) {
        GLOG_ERROR("failed to submit draw command buffer\n");
    }

    if (waitIdle) {
        vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());
    }
    
    if (freeCommandBuffer && !commandBuffersArray.empty()) {
        vkFreeCommandBuffers(vulkanDevice.getLogicalDevice(), commandPool, commandBuffersArray.size(), commandBuffersArray.data());
    }
}

#pragma mark - Routine -

VkCommandPool GCommandService::createCommandPool(GVULKANDevice& device) {
    VkCommandPoolCreateInfo poolInfo = { };
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = device.getGraphicsQueueIndex();
    
    VkCommandPool newCommandPool;
    if (vkCreateCommandPool(device.getLogicalDevice(), &poolInfo, nullptr, &newCommandPool) != VK_SUCCESS) {
        GLOG_ERROR("failed to create command pool\n");
    }
    
    return newCommandPool;
}

};
