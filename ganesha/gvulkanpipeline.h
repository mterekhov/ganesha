#ifndef SPCGANESHAENGINE_GVULKANPIPELINE_H
#define SPCGANESHAENGINE_GVULKANPIPELINE_H

#include <vulkan/vulkan.h>
#include <vector>

#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "gdescriptorsetservice.h"

namespace spcGaneshaEngine {

class GVULKANPipeline {
public:
    GVULKANPipeline();
    ~GVULKANPipeline();
    
    void createPipeline(GVULKANSwapChain& swapChain,
                        std::vector<VkPipelineShaderStageCreateInfo>& shadersArray,
                        std::shared_ptr<GDescriptorsetServiceProtocol> descriptorsetService,
                        GVULKANDevice& vulkanDevice);
    void destroyPipeline(GVULKANDevice& vulkanDevice);
    VkPipeline getGraphicsPipeline();
    VkPipelineLayout getPipelineLayout();

private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    VkPipelineRasterizationStateCreateInfo createRasterizer();
    VkPipelineLayout createPipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorsetLayout);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANPIPELINE_H
