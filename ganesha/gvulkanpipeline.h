#ifndef SPCGANESHAENGINE_GVULKANPIPELINE_H
#define SPCGANESHAENGINE_GVULKANPIPELINE_H

#include <vulkan/vulkan.h>
#include <array>

#include "glog.h"
#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "grendergraph.h"

namespace spcGaneshaEngine {

class GVULKANPipeline {
public:
    GVULKANPipeline();
    ~GVULKANPipeline();
    
    void createPipeline(GVULKANDevice& vulkanDevice, GVULKANSwapChain& swapChain, GRenderGraph& renderGraph, VkDescriptorSetLayout descriptorsetLayout);
    void destroyPipeline(GVULKANDevice& vulkanDevice);
    VkPipeline getGraphicsPipeline();
    VkPipelineLayout getPipelineLayout();

private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    VkPipelineVertexInputStateCreateInfo createVertexInput();
    VkPipelineRasterizationStateCreateInfo createRasterizer();
    VkPipelineLayout createPipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorsetLayout);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANPIPELINE_H
