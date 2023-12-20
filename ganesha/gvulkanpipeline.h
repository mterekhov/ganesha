#ifndef SPCGANESHAENGINE_GVULKANPIPELINE_H
#define SPCGANESHAENGINE_GVULKANPIPELINE_H

#include <vulkan/vulkan.h>
#include <array>

#include "glog.h"
#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "ganeshastubdata.h"

namespace spcGaneshaEngine {

class GVULKANPipeline {
public:
    GVULKANPipeline(GLog& log);
    ~GVULKANPipeline();
    
    void createPipeline(GVULKANDevice& vulkanDevice, GVULKANSwapChain& swapChain, VkDescriptorSetLayout descriptorsetLayout);
    void destroyPipeline(GVULKANDevice& vulkanDevice);
    VkPipeline getGraphicsPipeline();
    VkPipelineLayout getPipelineLayout();

private:
    GLog& log;
    VkVertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = Vertex::getAttributeDescriptions();
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    VkPipelineShaderStageCreateInfo createShader(const std::string& shaderFile,  const VkShaderStageFlagBits stage, VkDevice device);
    VkPipelineVertexInputStateCreateInfo createVertexInput();
    VkPipelineRasterizationStateCreateInfo createRasterizer();
    VkPipelineLayout createPipelineLayout(VkDevice device, VkDescriptorSetLayout& descriptorsetLayout);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANPIPELINE_H
