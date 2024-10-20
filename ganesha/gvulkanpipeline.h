#ifndef SPCGANESHAENGINE_GVULKANPIPELINE_H
#define SPCGANESHAENGINE_GVULKANPIPELINE_H

#include <vulkan/vulkan.h>
#include <array>

#include "glog.h"
#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "gdescriptorsetservice.h"
#include "gshadersservice.h"

namespace spcGaneshaEngine {

class GVULKANPipeline {
public:
    GVULKANPipeline();
    ~GVULKANPipeline();
    
    void createPipeline(GVULKANDevice& vulkanDevice, 
                        GVULKANSwapChain& swapChain,
                        std::vector<VkPipelineShaderStageCreateInfo>& shadersArray,
                        std::shared_ptr<GDescriptorsetServiceProtocol> descriptorsetService);
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
