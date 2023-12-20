#ifndef SPCGANESHAENGINE_GVULKANCOMMANDS_H
#define SPCGANESHAENGINE_GVULKANCOMMANDS_H

#include <vulkan/vulkan.h>

#include "glog.h"
#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "grendergraph.h"
#include "gvulkanpipeline.h"

namespace spcGaneshaEngine {

class GVULKANCommands {
public:
    GVULKANCommands(GLog& log);
    ~GVULKANCommands();
    
    void createCommands(GVULKANDevice& device);
    void destroyCommands(GVULKANDevice& device);

    VkCommandBuffer copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice device);
    VkCommandBuffer transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkDevice device);
    VkCommandBuffer emptyCommand(VkDevice device);
    VkCommandBuffer copyBufferCommand(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device);
    void recordRenderCommand(VkCommandBuffer renderCommand,
                             VkBuffer vertexesBuffer,
                             VkBuffer indexesBuffer,
                             const TUInt indexesNumber,
                             VkFramebuffer& framebuffer,
                             GVULKANSwapChain& swapChain,
                             GVULKANPipeline& pipeline,
                             VkDescriptorSet& descriptorset);
    void destroyCommandBuffer(VkCommandBuffer commandBuffer, VkDevice device);
    void submitCommand(VkCommandBuffer command, GVULKANDevice& vulkanDevice);

private:
    GLog& log;
    VkCommandPool commandPool;
    VkCommandPool createCommandPool(GVULKANDevice& device);
    VkCommandBuffer beginSingleCommand(VkDevice device);
    void endSingleCommand(VkCommandBuffer command);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANCOMMANDS_H
