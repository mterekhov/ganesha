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

    VkCommandPool& getCommandPool();

    VkCommandBuffer emptyCommand(const VkDevice& device);
    VkCommandBuffer copyBufferCommand(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkDevice& device);
    void recordRenderCommand(VkCommandBuffer& renderCommand,
                             VkBuffer& vertecesBuffer,
                             VkBuffer& indicesBuffer,
                             TUInt indicesNumber,
                             VkFramebuffer& framebuffer,
                             GVULKANSwapChain& swapChain,
                             GVULKANPipeline& pipeline,
                             VkDescriptorSet& descriptorset);
    void destroyCommandBuffer(VkCommandBuffer& commandBuffer, const VkDevice& device);

private:
    GLog& log;
    VkCommandPool commandPool;
    VkCommandPool createCommandPool(GVULKANDevice& device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANCOMMANDS_H
