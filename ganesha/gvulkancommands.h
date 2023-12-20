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

private:
    GLog& log;
    VkCommandPool commandPool;
    VkCommandPool createCommandPool(GVULKANDevice& device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANCOMMANDS_H
