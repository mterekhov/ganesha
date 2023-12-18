//
//  gvulkaninstance.hpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

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
    
    void createCommands(GVULKANDevice& device, GVULKANSwapChain& swapChain, GVULKANPipeline& pipline, const GRenderGraph& renderGraph);
    void destroyCommands(GVULKANDevice& device);
    VkCommandPool& getCommandPool();
    std::vector<VkCommandBuffer>& getCommandBuffersArray();

private:
    GLog& log;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffersArray;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkCommandPool createCommandPool(GVULKANDevice& device);
    std::vector<VkCommandBuffer> createCommandBuffers(GVULKANDevice& device, 
                                                      GVULKANSwapChain& swapChain,
                                                      GVULKANPipeline& pipeline,
                                                      VkCommandPool& currentCommandPool,
                                                      const GRenderGraph& renderGraph);
    VkBuffer createIndicesBuffer(GVULKANDevice& vulkanDevice, const std::vector<uint32_t>& indices);
    VkBuffer createVertexBuffer(GVULKANDevice& vulkanDevice, const std::vector<uint32_t>& vertices);
    void createBuffer(GVULKANDevice& vulkanDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyBuffer(GVULKANDevice& vulkanDevice, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANCOMMANDS_H
